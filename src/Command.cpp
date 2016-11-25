#include "Command.h"
#include "utils.h"
#include "Task.h"

#include <sstream>
#include <stdexcept>
#include <iostream>

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
        Arg task_id = Arg(ID, cmd_str);
        args.push_back(task_id);
        return;
    }

    if (cmd_str == "clear") {
        action = CLEAR;
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

int Command::get_id()
{
    int task_id;
    for (Arg arg : args) {
        if (arg.first == ID) {
            task_id = std::stoi(arg.second);
            break;
        }
    }

    if (!task_id) {
        throw std::invalid_argument("Task does not exist");
    }

    return task_id;
}

int Command::execute()
{
    switch (action) {
    case SHOW:
        return cmd_show_task();
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
    case CLEAR:
        return cmd_clear();
    default:
        return cmd_report();
    }
}

int Command::cmd_show_task()
{
    int task_id = get_id();
    Database db = Database(DB_PATH);
    Task task = db.get(task_id);
    std::cout << task;
    return 0;
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

int Command::cmd_done_task()
{
    int task_id = get_id();
    std::string sql = "UPDATE TASKS SET status = 1 WHERE id = "
                    + std::to_string(task_id) + ";";
    Database db = Database(DB_PATH);
    return db.execute_sql(sql);
}

int Command::cmd_modify_task()
{
    Database db = Database(DB_PATH);
    int rc;
    std::string task_id = std::to_string(get_id());
    std::string sql;

    for (Arg property : args) {
        if (property.first == DESC) {
            std::string description = "'" + property.second + "'";
            sql = "UPDATE TASKS SET description = " + description;
        }
        else if (property.first == DUE) {
            std::string due = "'" + property.second + "'";
            sql = "UPDATE TASKS SET due_date = " + due;
        }
        else if (property.first == PRIORITY) {
            std::string priority;
            if (property.second == "none") {
                priority = "0";
            }
            else if (property.second == "low") {
                priority = "1";
            }
            else if (property.second == "med") {
                priority = "2";
            }
            else if (property.second == "high") {
                priority = "3";
            }
            sql = "UPDATE TASKS SET priority = " + priority;
        }
        else if (property.first == TAG) {
            std::string tags = "'" + property.second + "'";
            sql = "UPDATE TASKS SET tags = tags || " + tags;
        }
        else {
            continue;
        }
        sql += " WHERE id = " + task_id + ";";

        if ((rc = db.execute_sql(sql)) != SQLITE_DONE) {
            break;
        }
    }
    return rc;
}

int Command::cmd_delete_task()
{
    int task_id = get_id();
    Database db = Database(DB_PATH);
    return db.remove_task(task_id);
}

int Command::cmd_clear()
{
    Database db = Database(DB_PATH);
    return db.remove_all();
}