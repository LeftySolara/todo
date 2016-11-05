#include "catch.hpp"
#include "Database.h"

#define DB_INIT_SCRIPT "../../scripts/init.sql"
#define DB_PATH "../test.db"

TEST_CASE("We can initialize the database", "[create]")
{
    Database db = Database(DB_PATH);
    REQUIRE_NOTHROW(db.execute_script(DB_INIT_SCRIPT));
}

TEST_CASE("We can modify the database", "[add][delete]")
{
    Database db = Database(DB_PATH);

    SECTION("We can add tasks to the database")
    {
        SECTION("Add tasks with only a description")
        {
            REQUIRE_NOTHROW(db.add_task("Buy milk");)
            REQUIRE_NOTHROW(db.add_task("Do homework for chapter 4");)
            REQUIRE_NOTHROW(db.add_task("Enter that one contest!");)
        }
        SECTION("Add tasks with a description and due date")
        {
            REQUIRE_NOTHROW(db.add_task("Read chapter 7 for class", "2016-11-05"));
            REQUIRE_NOTHROW(db.add_task("Back up home server", "2048-3-5"));
            REQUIRE_NOTHROW(db.add_task("Review schedule", "2014-04-3"));
            REQUIRE_NOTHROW(db.add_task("Fix heater", "2016-4-03"));
        }
    }
}
