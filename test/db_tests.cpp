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

TEST_CASE("We can add tasks to the database")
{
    Database db = Database(DB_PATH);
    std::vector<std::string> no_tags;
    std::vector<std::string> tags {"code", "testing", "fun"};
    db.add_task("Test task", "2016-11-16", 2, tags);
    db.add_task("another task", "2018-4-11", 1, no_tags);
}
