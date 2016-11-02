#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Database.h"

#define DB_INIT_SCRIPT "../../scripts/init.sql"

TEST_CASE("We can initialize a database")
{
    Database db = Database();
    REQUIRE(db.execute_script(DB_INIT_SCRIPT, "../test.db") == true);
}