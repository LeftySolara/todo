#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
using std::vector;
#include <string>
using std::string;

#include "Command.h"

TEST_CASE("We can view the command given to the object")
{
    Command cmd = Command();
    // cmd.parse("todo add buy milk");
}