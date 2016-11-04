#include "Database.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>

Database::Database(std::string path)
{
    db_path = path;
}

void Database::execute_script(std::string filename)
{
    connect();

    std::string line;
    std::ifstream script_file(filename);
    if (!script_file.is_open()) {
        throw sql_error;
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
                sqlite3_free(zErrMsg);
                sqlite3_close(db);
                throw sql_error;
            }
            sql_statement = "";
        }

    }

    sqlite3_close(db);
}

void Database::add_task(std::string desc, std::string due, int priority, std::vector<std::string> tags)
{
    connect();
    if (!is_valid_date(due)) {
        throw std::invalid_argument("Provided date is not it a valid Y-M-D format");
    }

    desc = "'" + desc + "'";
    due = "'" + due + "'";
    std::string status = "0, ";
    std::string tag_string = join_tags(tags);
    std::string sql = "INSERT INTO TASKS (description, due_date, priority, status, tags) " \
                      "VALUES (" + desc + ", " + due + ", " + std::to_string(priority) \
                      + ", " + status + "'" + tag_string + "');";

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        throw sql_error;
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

int Database::connect()
{
    rc = sqlite3_open(db_path.c_str(), &db);
    if (rc) {
        throw db_error;
    }
}

bool Database::is_valid_date(std::string date)
{
    bool valid = true;
    std::vector<std::string> tokens = split(date, '-');

    if (tokens.size() != 3) {
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

std::vector<std::string> Database::split(std::string str, char delim)
{
    std::string token;
    std::stringstream stream;
    std::vector<std::string> tokens;
    size_t pos = 0;

    stream.str(str);
    while (std::getline(stream, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string Database::join_tags(std::vector<std::string> v)
{
    std::string output = "";
    for (std::string s : v) {
        output = output + s;
        // Dont put a space after the last tag
        if (s != v.back()) {
            output = output + " ";
        }
    }
    return output;
}

int Database::callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
