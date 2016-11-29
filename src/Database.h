#include "Task.h"

#include <sqlite3.h>
#include <string>
#include <vector>


#ifndef DATABASE_H
#define DATABASE_H

class Database
{
public:
    Database(const std::string &path="../todo.sqlite");
    ~Database();

    void execute_script(const std::string &filename);
    int execute_sql(std::string statement);
    int add_task(Task tsk);
    int remove_task(const int task_id);
    int remove_all();
    Task get(const int task_id);
    std::vector<Task> get_all();
    int size();

private:
    int rc;
    char *zErrMsg = 0;
    sqlite3 *db;
    std::string db_path;

    int connect();
    static int callback(void *not_used, int num_cols, char **fields, char **col_names);

    int add_task(std::string desc);
    int add_task(std::string desc, std::string due);
    int add_task(std::string desc, std::string due, std::vector<std::string> tags);
    int add_task(std::string desc, std::string due, unsigned int priority);
    int add_task(std::string desc, std::vector<std::string> tags);
    int add_task(std::string desc, unsigned int priority, std::vector<std::string> tags);
    int add_task(std::string desc, unsigned int priority);
    int add_task(std::string desc, std::string due, unsigned int priority, std::vector<std::string> tags);

    bool is_valid_date(const std::string &date);
};

#endif
