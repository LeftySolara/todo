#include "catch.hpp"
#include "Task.h"

TEST_CASE("We can compare two tasks")
{
    std::vector<std::string> tags {"tag0", "tag1", "tag2"};
    
    Task t1 = {
        1,              // id
        false,          // done
        "First task",   // description
        "2016-04-05",   // due date
        low,            // priority
        tags            // tags
    };

    Task t2 = {
        2,              // id
        false,          // done
        "Second task",   // description
        "2016-04-05",   // due date
        low,            // priority
        tags            // tags
    };

    Task t3 = {
        1,              // id
        false,          // done
        "First task",   // description
        "2016-04-05",   // due date
        low,            // priority
        tags            // tags
    };

    Task t4 = {
        4,              // id
        false,          // done
        "First task",   // description
        "2016-04-05",   // due date
        low,            // priority
        tags            // tags
    };

    REQUIRE(t1 == t3);
    REQUIRE(t3 == t1);
    REQUIRE(t1 != t2);
    REQUIRE(t4 != t3);
}
