#include "Database.h"
#include "exceptions.h"
#include <fstream>

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
        throw db_error;
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
        if (sql_statement.back() == ';') {
            rc = sqlite3_exec(db, sql_statement.c_str(), callback, 0, &zErrMsg);
            if (rc != SQLITE_OK) {
                sqlite3_close(db);
                throw sql_error;
                return false;
            }
            sql_statement = "";
        }

    }

    sqlite3_close(db);
    return true;
}

int Database::callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
