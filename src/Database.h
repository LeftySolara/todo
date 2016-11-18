#include <sqlite3.h>
#include <string>
#include <vector>

#include "Task.h"

#ifndef DATABASE_H
#define DATABASE_H

class Database
{
public:
    Database(const std::string &path="../todo.sqlite");
    ~Database();   
    void execute_script(const std::string &filename);
    int add_task(Task tsk);
    int add_task(std::string desc);
    int add_task(std::string desc, std::string due);
    int add_task(std::string desc, std::string due, std::vector<std::string> tags);
    int add_task(std::string desc, std::string due, unsigned int priority);
    int add_task(std::string desc, std::vector<std::string> tags);
    int add_task(std::string desc, unsigned int priority, std::vector<std::string> tags);
    int add_task(std::string desc, unsigned int priority);
    int add_task(std::string desc, std::string due, unsigned int priority, std::vector<std::string> tags);
    int remove_task(int task_id);
    int remove_all();
    Task get(const int id);
    int status() const;
    // TODO: add method for closing the database connection manually
private:
    int rc;
    char *zErrMsg = 0;
    sqlite3 *db;
    std::string db_path;

    int connect();
    bool is_valid_date(const std::string &date);
    std::vector<std::string> split(const std::string &str, char delim=' ');
    std::string join_tags(std::vector<std::string> v);
    int execute_sql(std::string statement);
    static int callback(void *not_used, int num_cols, char **fields, char **col_names);
};

#endif
