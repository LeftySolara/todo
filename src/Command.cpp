#include "Command.h"
#include <stdexcept>
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
    std::string task_cmd = input_args.front();

    if (task_cmd == "add") {
        // User uses "add" command without any arguments
        if (input_args.size() == 1) {  
            throw std::invalid_argument("Not enough arguments for adding task");
            return;
        }

        verb = ADD;
        std::vector<std::string>::iterator it = input_args.begin()+1;
        filter_args(it, input_args.end());
    }
    else if (is_digits(task_cmd)) {
        Arg id = Arg(ID, task_cmd);
        args.push_back(id);

        std::string operation = input_args.at(1);
        if (operation == "delete") {
            verb = DEL;
        }
        else if (operation == "done") {
            verb = DONE;
        }
        else if (operation == "modify") {
            verb = MODIFY;
            std::vector<std::string>::iterator it = input_args.begin()+2;
            filter_args(it, input_args.end());
        }
    }
    else {
        throw std::invalid_argument("Invalid argument(s)")
    }
}

void Command::execute()
{
    switch (verb) {
    case SHOW:
        cmd_show_task();
        break;
    case SHOW_ALL:
        cmd_show_all();
        break;
    case ADD:
        cmd_add_task();
        break;
    case DONE:
        cmd_done_task();
        break;
    case MODIFY:
        cmd_modify_task();
        break;
    case DEL:
        cmd_delete_task();
        break;
    case REPORT:
        cmd_report();
        break;
    default:
        cmd_report();
    }
}

void Command::execute(std::string cmd)
{
    parse(cmd);
    execute();
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

void Command::filter_args(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end)
{
    Arg arg;
    std::string description = "";

    while (it != end) {
        if (it->find("+") == 0) {
            arg = Arg(TAG, it->substr(1));
            args.push_back(arg);
        }
        else if (it->find("due:") == 0) {
            arg = Arg(DUE, it->substr(4));
            args.push_back(arg);
        }
        else if (it->find("priority:") == 0) {
            arg = Arg(PRIORITY, it->substr(9));
            args.push_back(arg);
        }
        else {
            description = description + " " + *it;
        }
        ++it;
    }
    args.push_back(Arg(DESC, description));    
}
