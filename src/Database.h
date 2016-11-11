#include <sqlite3.h>
#include <string>
#include <vector>

#ifndef DATABASE_H
#define DATABASE_H

class Database
{
public:
    Database(const std::string &path);
    ~Database();   
    void execute_script(const std::string &filename);
    int add_task(std::string desc);
    int add_task(std::string desc, std::string due);
    int add_task(std::string desc, std::string due, std::vector<std::string> tags);
    int add_task(std::string desc, std::string due, unsigned int priority);
    int add_task(std::string desc, std::vector<std::string> tags);
    int add_task(std::string desc, unsigned int priority, std::vector<std::string> tags);
    int add_task(std::string desc, unsigned int priority);
    int add_task(std::string desc, std::string due, unsigned int priority, std::vector<std::string> tags);
    int remove_task(int task_id);
private:
    int rc;
    char *zErrMsg = 0;
    sqlite3 *db;
    std::string db_path;

    int connect();
    bool is_valid_date(const std::string &date);
    std::vector<std::string> split(const std::string &str, char delim);
    std::string join_tags(std::vector<std::string> v);
    int execute_sql(std::string statement);
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
};

#endif
