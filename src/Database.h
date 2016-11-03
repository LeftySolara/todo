#include <sqlite3.h>
#include <string>
#include <vector>

#ifndef DATABASE_H
#define DATABASE_H

class Database
{
public:
    Database(std::string path);
    // ~Database();   
    void execute_script(std::string filename);
    void add_task(std::string desc, std::string due, int priority, std::vector<std::string> tags);
private:
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;
    std::string db_path;
    int connect();
    bool is_valid_date(std::string date);
    std::vector<std::string> split(std::string str, char delim);
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
};

#endif
