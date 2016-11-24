#include "Command.h"
#include "utils.h"
#include "Task.h"

#include <sstream>
#include <stdexcept>

#define DB_PATH "../test.sqlite"

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

    // When the action is "add", the next word is part of the task description,
    // so don't delete it
    if (cmd_verb != "add") {
        cmd_args.erase(cmd_args.begin());
    }

    filter_args(cmd_args);
}

void Command::filter_args(const std::vector<std::string> &tokens)
{
    Arg argument;
    std::string description = "";

    for (std::string token : tokens) {
        if (token.find("due:") == 0) {
            argument = Arg(DUE, token.substr(4));
            args.push_back(argument);
        }
        else if (token.find("priority:") == 0) {
            argument = Arg(PRIORITY, token.substr(9));
            args.push_back(argument);
        }
        else if (token.find("+") == 0) {
            argument = Arg(TAG, token.substr(1));
            args.push_back(argument);
        }
        else {
            description = description + " " + token;
        }
    }

    // Remove the leading space character
    if (!description.empty()) {
        description.erase(description.begin());
        argument = Arg(DESC, description);
    }
    args.push_back(argument);
}

int Command::execute()
{
    switch (action) {
    case SHOW:
        return cmd_show_task();
    case SHOW_ALL:
        return cmd_show_all();
    case ADD:
        return cmd_add_task();
    case DONE:
        return cmd_done_task();
    case MODIFY:
        return cmd_modify_task();
    case DEL:
        return cmd_delete_task();
    case REPORT:
        return cmd_report();
    default:
        return cmd_report();
    }
}

int Command::cmd_add_task()
{
    Task tsk;
    Property prop;
    std::string val;
    std::vector<std::string> tags;

    // We don't care about the task's ID or completion status here.
    // The ID is generated by the SQLite database and the status defaults to
    // "not done" on the database insert.
    for (Arg argument : args) {
        prop = argument.first;
        val = argument.second;

        if (prop == DESC) {
            tsk.description = val;
        }
        else if (prop == DUE) {
            tsk.due_date = val;
        }
        else if (prop == PRIORITY) {
            if (val == "low") {
                tsk.priority = (Priority)1;
            }
            else if (val == "med") {
                tsk.priority = (Priority)2;
            }
            else if (val == "high") {
                tsk.priority = (Priority)3;
            }
            else {
                throw std::invalid_argument("Invalid value for priority");
            }
        }
        else if (prop == TAG) {
            tags.push_back(val);
        }
    }

    tsk.tags = tags;
    Database db = Database(DB_PATH);
    return db.add_task(tsk);
}
