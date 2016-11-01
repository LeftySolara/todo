#include <sqlite3.h>
#include <string>

#ifndef DATABASE_H
#define DATABASE_H

class Database
{
public:
    Database();
    // ~Database();   
    bool execute_script(std::string filename, std::string db_path);
private:
    sqlite3 *db;
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
};

#endif
