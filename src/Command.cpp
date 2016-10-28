#include "Command.h"
#include <sstream>
#include <algorithm>

Command::Command()
{
    parse("");
}

Command::Command(std::string cmd)
{
    parse(cmd);
}

void Command::parse(std::string cmd)
{
    args.clear();

    if (cmd.empty()) {
        verb = REPORT;
    }

    // If user only provides an id, show the details of that task
    if (is_digits(cmd)) {
        verb = SHOW;
        Arg task_id = Arg(ID, cmd);
        args.push_back(task_id);
        return;
    }

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), tolower);
    std::vector<std::string> input_args = split_str(cmd);

    if (input_args[0] == "add") {
        // User uses "add" command without any arguments
        // TODO: Raise error here
        if (input_args.size() == 1)
            return;

        input_args.erase(input_args.begin());
        std::string description = "";
        Arg arg;

        for (std::string str : input_args) {
            if (str.find("+") == 0) {
                arg = Arg(TAG, str.substr(1));
                args.push_back(arg);
            }
            else if (str.find("due:") == 0) {
                arg = Arg(DUE, str.substr(4));
                args.push_back(arg);
            }
            else if (str.find("priority:") == 0) {
                arg = Arg(PRIORITY, str.substr(9));
                args.push_back(arg);
            }
            else
                description = description + " " + str;
        }
        args.push_back(Arg(DESC, description));
    }
}

std::vector<std::string> Command::split_str(const std::string &str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    for (std::string s; iss >> s;)
        result.push_back(s);

    return result;
}

bool Command::is_digits(const std::string str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}
