#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
using std::vector;
#include <string>
using std::string;

#include "Command.h"

TEST_CASE("We can create and initialize command objects")
{
    SECTION("A Command object can be created without an initialization string")
    {
        Command c = Command();
        REQUIRE(c.cmd() == "todo show");
    }

    SECTION("A command object can be created with only a description")
    {
        string user_input = "todo add buy milk";
        Command c = Command(user_input);
        REQUIRE(c.cmd() == user_input);
    }

    SECTION("A command object can be created with tags")
    {
        string user_input = "todo add buy milk +shopping";
        Command c = Command(user_input);
        REQUIRE(c.cmd() == user_input);
    }

    SECTION("A command object can be created with a due date")
    {
        string user_input = "todo add buy milk due:2016-10-29";
        Command c = Command(user_input);
        REQUIRE(c.cmd() == user_input);
    }

    SECTION("A command object can be created with a priority")
    {
        string user_input = "todo add buy milk priority:low";
        Command c = Command(user_input);
        REQUIRE(c.cmd() == user_input);
    }

    SECTION("A command object can be created with tags, a due date, and a priority")
    {
        string user_input = "todo add buy milk +shopping priority:low due:2016-10-29";
        Command c = Command(user_input);
        REQUIRE(c.cmd() == user_input);
    }
}
