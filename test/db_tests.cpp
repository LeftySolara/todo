#include "catch.hpp"
#include "Database.h"
#include "Task.h"

#define DB_INIT_SCRIPT "../../scripts/init.sql"
#define DB_PATH "../test.db"

TEST_CASE("We can initialize the database", "[create]")
{
    Database db = Database(DB_PATH);
    REQUIRE_NOTHROW(db.execute_script(DB_INIT_SCRIPT));
}

TEST_CASE("We can modify the database", "[add][remove]")
{
    Database db = Database(DB_PATH);
    std::vector<std::string> input_tags {"tag0", "tag1", "tag2", "tag3", "tag4"};

    SECTION("We can add tasks to the database")
    {
        SECTION("Add tasks using a Task object")
        {
            Task t1 = {
                1,              // id
                false,          // done
                "First task",   // description
                "2016-04-05",   // due date
                low,              // priority
                input_tags      // tags
            };
            REQUIRE(db.add_task(t1) == SQLITE_OK);
        }
        SECTION("Add tasks with only a description")
        {
            REQUIRE(db.add_task("Buy milk") == SQLITE_OK);
            REQUIRE(db.add_task("Do homework for chapter 4") == SQLITE_OK);
            REQUIRE(db.add_task("Enter that one contest!") == SQLITE_OK);
        }
        SECTION("Add tasks with a description and due date")
        {
            // Also test for different formats of date strings
            REQUIRE(db.add_task("Read chapter 7 for class", "2016-11-05") == SQLITE_OK);
            REQUIRE(db.add_task("Back up home server", "2048-3-5") == SQLITE_OK);
            REQUIRE(db.add_task("Review schedule", "2014-04-3") == SQLITE_OK);
            REQUIRE(db.add_task("Fix heater", "2016-4-03") == SQLITE_OK);
        }
        SECTION("Add tasks with a description, due date, and tags")
        {
            REQUIRE(db.add_task("Write term paper", "2016-04-05", input_tags) == SQLITE_OK);
        }
        SECTION("Add tasks with a description, due date, and priority")
        {
            REQUIRE(db.add_task("Call mom", "2016-05-03", high) == SQLITE_OK);
        }
        SECTION("Add tasks with a description and tags")
        {
            REQUIRE(db.add_task("Buy new shoes", input_tags) == SQLITE_OK);
        }
        SECTION("Add tasks with a description, priority, and tags")
        {
            REQUIRE(db.add_task("Fix firewall rules", med, input_tags) == SQLITE_OK);
        }
        SECTION("Add tasks with a description and priority")
        {
            REQUIRE(db.add_task("Finish that thing", med) == SQLITE_OK);
        }
        SECTION("Add tasks with a description, due date, priority, and tags")
        {
            REQUIRE(db.add_task("Do the thing", "2016-07-06", low, input_tags) == SQLITE_OK);
        }

        SECTION("We can't add tasks with invalid date formats")
        {
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "2016"), std::invalid_argument);
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "2016-05"), std::invalid_argument);
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "tomorrow"), std::invalid_argument);
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "12345"), std::invalid_argument);
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "2016-02-31"), std::invalid_argument);
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "2016-05-0555"), std::invalid_argument);
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "2016--05-5"), std::invalid_argument);
            REQUIRE_THROWS_AS(db.add_task("Read chapter 7 for class", "!@#$&^"), std::invalid_argument);
        }
    }

    SECTION("We can remove tasks from the database")
    {
        SECTION("Tasks that exist are removed")
        {
            REQUIRE(db.remove_task(3) == SQLITE_OK);
            REQUIRE(db.remove_task(2) == SQLITE_OK);
        }
    }
    SECTION("We can clear the database")
    {
        REQUIRE(db.remove_all() == SQLITE_OK);
    }
}

TEST_CASE("We can fetch tasks from the database")
{
    Database db = Database(DB_PATH);
    std::vector<std::string> input_tags {"tag0", "tag1", "tag2", "tag3", "tag4"};

    Task t1 = {
        1,              // id
        false,          // done
        "First task",   // description
        "2016-04-05",   // due date
        low,            // priority
        input_tags      // tags
    };

    db.add_task(t1);
    REQUIRE(t1 == db.get(t1.id));
}
