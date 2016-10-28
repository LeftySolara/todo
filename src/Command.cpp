#include "Command.h"
#include <sstream>
#include <algorithm>
#include <cctype>

Command::Command()
{
    parse("todo show");
}

Command::Command(const std::string &cmd)
{
    parse(cmd);
}

void Command::parse(std::string &cmd)
{
    cmd = std::transform(cmd.begin(), cmd.end(), cmd.begin(), tolower);

    // Default to just showing all tasks
    if (cmd.length() == 0 || cmd == "show" {
        cmd_show_all();
        return;
    }

    std::vector<std::string> input_args = split_str(cmd);

    // std::string input_verb = input_args[0];
    // if (input_verb == "show")
    //     verb = show;
    // else if (input_verb == "add")
    //     verb = add
    // else if (input_verb == "done")
    //    verb = done
    // else if (input_verb == "modify")
    //     verb = modify
    // else if (input_verb == "delete")
    //     verb = del
    // else
    //     verb = show
    // input_args.erase(input_args.begin());
}

std::vector<string> Command::split_str(const std::string &str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    for (std::string s; iss >> s;)
        result.push_back(s);

    return result;
}

bool is_digits(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}