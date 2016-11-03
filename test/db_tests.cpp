#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Database.h"

#define DB_INIT_SCRIPT "../../scripts/init.sql"
#define DB_PATH "../test.db"

TEST_CASE("We can initialize a database")
{
    Database db = Database(DB_PATH);
    db.execute_script(DB_INIT_SCRIPT);
}
