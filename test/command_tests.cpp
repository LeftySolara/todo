#include "catch.hpp"
#include "Command.h"
#include "Task.h"

TEST_CASE("We can initialize Command objects", "[init]")
{
    Command cmd = Command();
    Command cmd2 = Command("add Hello World");
}

TEST_CASE("We can add tasks to the database", "[add]")
{
    Command cmd = Command();

    SECTION("We can add a task with only a description")
    {
        cmd.parse("add task with description");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can add a task with a description and due date")
    {
        cmd.parse("add task description and due date due:2016-05-07");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can add a task with a description, due date, and tags")
    {
        cmd.parse("add task with description, due date, and tags due:2016-06-06 +tags +test +due");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can add a task with a description, due date, and priority")
    {
        cmd.parse("add task with description, due date, and low priority due:2016-04-04 priority:low");
        REQUIRE(cmd.execute() == SQLITE_DONE);
        cmd.parse("add task with description, due date, and medium priority due:2016-04-04 priority:med");
        REQUIRE(cmd.execute() == SQLITE_DONE);
        cmd.parse("add task with description, due date, and high priority due:2016-04-04 priority:high");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can add a task with a description and tags")
    {
        cmd.parse("add task with description and tags +test +section +asdf");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can add a task with a description, priority, and tags")
    {
        cmd.parse("add task with description, priority, and tags +test +section +asdf priority:med");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can add a task with a description and priority")
    {
        cmd.parse("add task with description and priority priority:med");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can add a task with a description, due date, priority, and tags")
    {
        cmd.parse("add task with everything priority:med due:2017-08-10 +test +tags +everything");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can't add tasks with invalid due dates")
    {
        std::vector<std::string> dates {"2016", "2016-05", "tomorrow", "12345",
                                        "2016-02-31", "2016-05-0555", "2016--05-5",
                                        "2016-4-5", "!@#$&^"};
        for (std::string date : dates) {
            cmd.parse("add task with bad date due:" + date);
            REQUIRE_THROWS_AS(cmd.execute(), std::invalid_argument);
        }
    }
}

TEST_CASE("We can remove tasks from the database", "[remove][delete]")
{
    Command cmd = Command("1 delete");

    SECTION("Existing tasks can be deleted")
    {
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("If a task doesn't exist, command does nothing")
    {
        cmd.parse("8888 delete");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
    SECTION("We can clear the database")
    {
        cmd.parse("clear");
        REQUIRE(cmd.execute() == SQLITE_DONE);
    }
}

TEST_CASE("We can get print tasks info to the screen")
{
    Command cmd = Command("add buy new shoes due:2016-12-12 priority:low +shopping");
    cmd.execute();

    cmd.parse("1");
    REQUIRE_NOTHROW(cmd.execute());
}