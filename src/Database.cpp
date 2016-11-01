#include "Database.h"
#include <fstream>
#include <string>

Database::Database()
{

}

bool Database::execute_script(std::string filename, std::string db_path)
{
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(db_path.c_str(), &db);
    if(rc) {
        // Couldn't open database
        // TODO: raise error here
        return false;
    }

    std::string line;
    std::ifstream script_file(filename);
    if (!script_file.is_open()) {
        return false;
    }

    std::string sql_statement = "";
    while (std::getline(script_file, line)) {
        // ignore comments and blank lines
        if (line.find("--") == 0 || line.length() == 0) {
            continue;
        }

        sql_statement = sql_statement + line;
        // if (sql_statement.back() == ';') {
        //     rc = sqlite3_exec(db, sql_statement, callback, 0, &zErrMsg);
        //     if (rc != SQLITE_OK) {
        //         // TODO: raise error here
        //         sqlite3_close(db);
        //         return false;
        //     }
        // }

    }

    sqlite3_close(db);

    return true;
}

static int callback()
{
    
}
