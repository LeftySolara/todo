#include "Command.h"
#include "Task.h"
#include <cctype>
#include <sstream>
using std::istringstream;
#include <algorithm>
using std::transform;

Command::Command() : action(SHOW)
{
}

Command::Command(string input)
{
    parse(input);
}

// Parse user input, identify arguments, and save the values
void Command::parse(string input)
{
    transform(input.begin(), input.end(), input.begin(), tolower);
    vector<string> tokens = split_string(input);

    if (tokens[0] == "todo")
        tokens.erase(tokens.begin());

    string verb = tokens[0];
    if (verb == "show")
        action = SHOW;
    else if (verb == "add")
        action = ADD;
    else if (verb == "modify")
        action = MODIFY;
    else if (verb == "done")
        action = DONE;
    else if (verb == "del")
        action = DEL;
    else
        action = SHOW;
    tokens.erase(tokens.begin());

    for (string token : tokens) {
        Arg arg;

        if (token.find("due:") == 0)
            arg.arg_type = DATE;
        else if (token.find("priority:") == 0)
            arg.arg_type = PRIORITY;
        else if (token.find("+") == 0)
            arg.arg_type = TAG;
        else
            arg.arg_type = DESCRIPTION;

        arg.value = token;
        args.push_back(arg);
    }
}

void Command::execute()
{
    switch (action) {
        case SHOW:
            showTask();
            break;
        case ADD:
            addTask();
            break;
        case MODIFY:
            modifyTask();
        case DONE:
            doneTask();
        case DEL:
            deleteTask();
        default:
            showTask();
    }
}

void Command::execute(string input)
{
    parse(input);
    execute();
}

// Return the last command given to the object
string Command::cmd()
{
    string cmd_str = "todo";

    switch (action) {
        case SHOW:
            cmd_str.append(" show");
            break;
        case ADD:
            cmd_str.append(" add");
            break;
        case MODIFY:
            cmd_str.append(" modify");
            break;
        case DONE:
            cmd_str.append(" done");
            break;
        case DEL:
            cmd_str.append(" del");
            break;
    }

    if (!args.empty()) {
        for (Arg a : args)
            cmd_str.append(" " + a.value);
    }

    return cmd_str;
}

void Command::showTask()
{
    return;
}

void Command::addTask()
{
    Task task = Task("test string");
}

void Command::modifyTask()
{
    return;
}

void Command::doneTask()
{
    return;
}

void Command::deleteTask()
{
    return;
}

// Split given string on each space character
vector<string> Command::split_string(string str)
{
    vector<string> result;
    istringstream iss(str);
    for (string s; iss >> s; )
        result.push_back(s);

    return result;
}
