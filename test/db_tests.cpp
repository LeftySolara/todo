#include "catch.hpp"
#include "Database.h"
#include "Task.h"

#define DB_INIT_SCRIPT "../../scripts/init.sql"
#define DB_PATH "../test.sqlite"

TEST_CASE("We can initialize the database", "[create]")
{
    Database db = Database(DB_PATH);
    REQUIRE_NOTHROW(db.execute_script(DB_INIT_SCRIPT));
}

TEST_CASE("We can modify the database", "[add][remove]")
{
    Database db = Database(DB_PATH);
    std::vector<std::string> no_tags;
    std::vector<std::string> user_tags {"tag0", "tag1", "tag2", "tag3", "tag4"};

    SECTION("We can add tasks to the database")
    {
        // The ID and DONE fields don't matter when calling add_task() since
        // the database handles those for us; we just need to make sure
        // everything gets initialized.
        Task task = {
            1,          // ID
            false,      // done
            "",         // description
            "",         // due date
            none,       // priority
            no_tags     // tags
        };

        SECTION("Add task with just a description")
        {
            task.description = "Just a description";
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
        SECTION("Add task with a description and due date")
        {
            task.description = "Description and due date";
            task.due_date = "2016-12-04";
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
        SECTION("Add task with a description, due date, and tags")
        {
            task.description = "Description, due date, and tags";
            task.due_date = "2016-04-02";
            task.tags = user_tags;
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
        SECTION("Add task with a description, due date, and priority")
        {
            task.description = "Description, due date, and priority";
            task.due_date = "2017-05-06";
            task.priority = med;
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
        SECTION("Add task with a description and tags")
        {
            task.description = "Description and tags";
            task.tags = user_tags;
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
        SECTION("Add task with a description, priority, and tags")
        {
            task.description = "Description, priority, and tags";
            task.priority = low;
            task.tags = user_tags;
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
        SECTION("Add task with a description and priority")
        {
            task.description = "Description and priority";
            task.priority = high;
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
        SECTION("Add task with a description, due date, priority, and tags")
        {
            task.description = "Description, due date, priority, and tags";
            task.due_date = "2016-12-06";
            task.priority = low;
            task.tags = user_tags;
            REQUIRE(db.add_task(task) == SQLITE_DONE);
        }
    }
    SECTION("We can't add tasks with invalid arguments")
    {
        Task task = {
            1,          // ID
            false,      // done
            "",         // description
            "",         // due date
            none,       // priority
            no_tags     // tags
        };

        SECTION("We can't add a task without a description")
        {
            REQUIRE_THROWS_AS(db.add_task(task), std::invalid_argument);
        }
        SECTION("We can't add a task with an invalid due date")
        {
            std::vector<std::string> dates {"2016", "2016-05", "tomorrow", "12345",
                                            "2016-02-31", "2016-05-0555", "2016--05-5",
                                            "2016-4-5", "!@#$&^"};
            for (std::string date : dates) {
                task.due_date = date;
                REQUIRE_THROWS_AS(db.add_task(task), std::invalid_argument);
            }
        }
    }
    SECTION("We can remove tasks from the database")
    {
        SECTION("Tasks that exist are removed")
        {
            REQUIRE(db.remove_task(3) == SQLITE_DONE);
            REQUIRE(db.remove_task(2) == SQLITE_DONE);
        }
    }
    SECTION("We can clear the database")
    {
        REQUIRE(db.remove_all() == SQLITE_DONE);
    }
}

TEST_CASE("We can fetch tasks from the database")
{
    Database db = Database(DB_PATH);
    std::vector<std::string> user_tags {"tag0", "tag1", "tag2", "tag3", "tag4"};

    Task t1 = {
        1,              // id
        false,          // done
        "First task",   // description
        "2016-04-05",   // due date
        low,            // priority
        user_tags       // tags
    };

    Task t2 = {
        2,
        false,
        "Second task",
        "2016-04-07",
        low, 
        user_tags
    };

    db.remove_all();
    db.add_task(t1);
    db.add_task(t2);
    REQUIRE(t1 == db.get(t1.id));
    REQUIRE(t2 == db.get(t2.id));
    REQUIRE(t1 != db.get(t2.id));
    REQUIRE(t2 != db.get(t1.id));

    SECTION("We get an empty task when requesting an id that doesn't exist")
    {
        Task t3 = db.get(12345);
        REQUIRE(t3.id == 0);
    }

    SECTION("We can get the size of the database")
    {
        db.add_task(t2);
        REQUIRE(db.size() == 3);
    }
}
