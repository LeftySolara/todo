#include "Database.h"
#include "utils.h"

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdexcept>

#define DB_INIT_SCRIPT "../../scripts/init.sql"


Database::Database(const std::string &path)
{
    // Create the database table if it doesn't exist
    db_path = path;
    execute_script(DB_INIT_SCRIPT);
}

Database::~Database()
{
    // We don't need to check if db is empty/open, as passing a null pointer
    // to sqlite3 memory allocation methods is harmless.
    sqlite3_free(zErrMsg);
    rc = sqlite3_close(db);
    if (rc == SQLITE_BUSY) {
        fprintf(stderr, "Could not close database: %s\n", sqlite3_errmsg(db));
    }
}

// Read a SQL script line-by-line and execute its commands
void Database::execute_script(const std::string &filename)
{
    if (connect() != SQLITE_OK) {
        return;
    }

    std::string line;
    std::ifstream script_file(filename);
    if (!script_file.is_open()) {
        fprintf(stderr, "Error: Could not open script file %s\n", filename);
        return;
    }

    std::string sql_statement = "";
    while (std::getline(script_file, line)) {
        // ignore comments and blank lines
        if (line.find("--") == 0 || line.length() == 0) {
            continue;
        }

        sql_statement = sql_statement + line;
        if (sql_statement.back() == ';') {
            rc = sqlite3_exec(db, sql_statement.c_str(), callback, 0, &zErrMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Can't execute sql statement: %s\n", sqlite3_errmsg(db));
                return;
            }
            sql_statement = "";
        }

    }
}

int Database::add_task(Task tsk)
{
    if (tsk.description.empty()) {
        throw std::invalid_argument("Task must have a description");
    }

    // We don't need to check for the task ID or status. The sqlite database
    // generates IDs on insert and all tasks default to "not done".
    bool has_due = !tsk.due_date.empty();
    bool has_priority = tsk.priority == low || tsk.priority == med || tsk.priority == high;
    bool has_tags = tsk.tags.size() > 0;

    if (has_due && has_priority && has_tags) {
        return add_task(tsk.description, tsk.due_date, tsk.priority, tsk.tags);
    }
    else if (has_due && has_priority && !has_tags) {
        return add_task(tsk.description, tsk.due_date, tsk.priority);
    }
    else if (has_due && !has_priority && has_tags) {
        return add_task(tsk.description, tsk.due_date, tsk.tags);
    }
    else if (!has_due && has_priority && has_tags) {
        return add_task(tsk.description, tsk.priority, tsk.tags);
    }
    else if (has_due && !has_priority && !has_tags) {
        return add_task(tsk.description, tsk.due_date);
    }
    else if (!has_due && has_priority && !has_tags) {
        return add_task(tsk.description, tsk.priority);
    }
    else if (!has_due && !has_priority && has_tags) {
        return add_task(tsk.description, tsk.tags);
    }
    else if (!has_due && !has_priority && !has_tags) {
        return add_task(tsk.description);
    }
}

int Database::remove_task(const int task_id)
{
    if (connect() != SQLITE_OK) {
        return rc;
    }

    std::string sql = "DELETE from TASKS where id=" + std::to_string(task_id) + ";";
    return execute_sql(sql);
}

int Database::remove_all()
{
    if (connect() != SQLITE_OK) {
        return rc;
    }

    return execute_sql("DELETE FROM TASKS;");
}

Task Database::get(const int task_id)
{
    Task tsk;

    // Return an empty object if we can't connect to the database
    if (connect() != SQLITE_OK) {
        return tsk;
    }

    sqlite3_stmt *stmt;
    std::string sql = "SELECT * FROM TASKS WHERE id=" + std::to_string(task_id);
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SELECT failed: %s\n", sqlite3_errmsg(db));
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        tsk.id = sqlite3_column_int(stmt, 0);
        tsk.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        tsk.due_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        tsk.priority = (Priority)sqlite3_column_int(stmt, 3);
        tsk.done = sqlite3_column_int(stmt, 4);
        tsk.tags = utils::split(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SELECT failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return tsk;
}

std::vector<Task> Database::get_all()
{
    std::vector<Task> tasks;
    Task task;
    std::string desc;
    int i = 1;

    while ((desc = get(i).description) != "") {
        task = get(i++);
        tasks.push_back(task);
    }

    return tasks;
}

int Database::connect()
{
    rc = sqlite3_open(db_path.c_str(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    return rc;
}

// Execute the provided SQL statement. Not meant to be used for SELECTs.
// Returns SQLITE_DONE on successful completion.
int Database::execute_sql(std::string statement)
{
    if (statement.empty()) {
        throw std::invalid_argument("Cannot execute empty SQL command");
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, statement.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);

    return rc;
}

// The callback is called for each fow in the result set of a SELECT statement
int Database::callback(void *not_used, int num_cols, char **fields, char **col_names) {
   int i;
   for(i = 0; i < num_cols; i++) {
      printf("%s = %s\n", col_names[i], fields[i] ? fields[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int Database::add_task(std::string desc)
{
    if (desc.empty()) {
        throw std::invalid_argument("Cannot add task with empty description");
    }

    desc = "'" + desc + "'";
    std::string sql = "INSERT INTO TASKS (description) VALUES (" + desc + ");";
    return execute_sql(sql);
}

int Database::add_task(std::string desc, std::string due)
{
    if (!is_valid_date(due)) {
        throw std::invalid_argument("Invalid date format");
    }

    desc = "'" + desc + "'";
    due = "'" + due + "'";
    std::string sql = "INSERT INTO TASKS (description, due_date) VALUES ("
                    + desc + ", date(" + due + "));";

    return execute_sql(sql);
}

int Database::add_task(std::string desc, std::string due, std::vector<std::string> tags)
{
    if (!is_valid_date(due)) {
        throw std::invalid_argument("Invalid date format");
    }

    desc = "'" + desc + "'";
    due = "'" + due + "'";
    std::string tag_string = "'" + utils::join(tags) + "'";
    std::string sql = "INSERT INTO TASKS (description, due_date, tags) VALUES ("
                    + desc + ", date(" + due + "), " + tag_string + ");";

    return execute_sql(sql);
}

int Database::add_task(std::string desc, std::string due, unsigned int priority)
{
    if (!is_valid_date(due)) {
        throw std::invalid_argument("Invalid date format");
    }

    desc = "'" + desc + "'";
    due = "'" + due + "'";
    std::string sql = "INSERT INTO TASKS (description, due_date, priority) VALUES ("
                    + desc + ", date(" + due + "), " + std::to_string(priority) + ");";

    return execute_sql(sql);
}

int Database::add_task(std::string desc, std::vector<std::string> tags)
{
    desc = "'" + desc + "'";
    std::string tag_string = "'" + utils::join(tags) + "'";
    std::string sql = "INSERT INTO TASKS (description, tags) VALUES ("
                    + desc + ", " + tag_string + ");";

    return execute_sql(sql);
}

int Database::add_task(std::string desc, unsigned int priority, std::vector<std::string> tags)
{
    desc = "'" + desc + "'";
    std::string tag_string = "'" + utils::join(tags) + "'";
    std::string sql = "INSERT INTO TASKS (description, priority, tags) VALUES ("
                    + desc + ", " + std::to_string(priority) + ", " + tag_string + ");";

    return execute_sql(sql);
}

int Database::add_task(std::string desc, unsigned int priority)
{
    desc = "'" + desc + "'";
    std::string sql = "INSERT INTO TASKS (description, priority) VALUES ("
                    + desc + ", " + std::to_string(priority) + ");";

    return execute_sql(sql);
}

int Database::add_task(std::string desc, std::string due, unsigned int priority, std::vector<std::string> tags)
{
    if (!is_valid_date(due)) {
        throw std::invalid_argument("Invalid date format");
    }

    desc = "'" + desc + "'";
    due = "'" + due + "'";
    std::string tag_string = "'" + utils::join(tags) + "'";
    std::string sql = "INSERT INTO TASKS (description, due_date, priority, tags) VALUES ("
                    + desc + ", date(" + due + "), " + std::to_string(priority) + ", " + tag_string + ");";

    return execute_sql(sql);
}

bool Database::is_valid_date(const std::string &date)
{
    bool valid = true;
    std::vector<std::string> tokens = utils::split(date, '-');

    if (tokens.size() != 3) {
        valid = false;
        return valid;
    }

    if (tokens.at(1).length() != 2 || tokens.at(2).length() != 2) {
        valid = false;
        return valid;
    }

    int year = stoi(tokens[0]);
    int month = stoi(tokens[1]);
    int day = stoi(tokens[2]);

    if (year < 1900 || year > 32000) {
        valid = false;
    }
    if (day <= 0) {
        valid = false;
    }

    switch (month) {
    case 2:
        if ((year % 4) == 0) {
            if (day > 29) {
                valid = false;
            }
        }
        else {
            if (day > 28) {
                valid = false;
            }
        }
        break;
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        if (day > 31) {
            valid = false;
        }
        break;
    case 4: case 6: case 9: case 11:
        if (day > 30) {
            valid = false;
        }
        break;
    default:
        valid = false;
        break;
    }

    return valid;
}
