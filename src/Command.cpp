#include "Command.h"
#include "utils.h"

#include <sstream>
#include <stdexcept>

Command::Command()
{
    cmd = "";
    parse(cmd);
}

Command::Command(std::string cmd_str)
{
    cmd = cmd_str;
    parse(cmd);
}

void Command::parse(std::string cmd_str)
{
    args.clear();
    if (cmd_str.empty()) {
        action = REPORT;
        return;
    }

    // If user only provides an id, show the details of that task
    if (utils::is_digits(cmd_str)) {
        action = SHOW;
        Arg task_id = Arg(ID, cmd);
        args.push_back(task_id);
        return;
    }

    std::vector<std::string> cmd_args = utils::split(cmd_str);
    std::string cmd_verb = cmd_args.front();
    utils::to_lower(cmd_verb);
    cmd_args.erase(cmd_args.begin());  // remove the verb so we can just work with the arguments

    if (cmd_verb == "add") {
        // User uses "add" command without any arguments
        if (cmd_args.empty()) {  
            throw std::invalid_argument("Not enough arguments for adding task");
            return;
        }
        action = ADD;
    }
    // If first argument is an ID number, we can only delete, modify, or mark done
    else if (utils::is_digits(cmd_verb)) {
        std::string operation = cmd_args.front();
        utils::to_lower(operation);
        if (operation == "delete") {
            action = DEL;
        }
        else if (operation == "done") {
            action = DONE;
        }
        else if (operation == "modify") {
            action = MODIFY;
        }
        else {
            throw std::invalid_argument("Invalid operation for task");
        }

        Arg task_id = Arg(ID, cmd_verb);
        args.push_back(task_id);
    }
    else {
        throw std::invalid_argument("Invalid argument(s)");
    }

    cmd_args.erase(cmd_args.begin());
    // TODO: assign properties to arguments
}
