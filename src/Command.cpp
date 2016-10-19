#include "Command.h"

Command::Command() : action(show)
{
}

Command::Command(string input)
{
    parse(input);
}

vector<string> Command::split_string(string str)
{
    
}

void Command::parse(string input)
{
    vector<string> tokens = split_string(input);

    switch (tokens[1]) {
        case SHOW:
            action = SHOW;
        case ADD:
            action = ADD;
        case MODIFY:
            action = MODIFY;
        case DONE:
            action = DONE;
        case DEL:
            action = DEL;
        default:
            action = SHOW;
    }

    for (token : tokens) {
        Arg arg;

        if (token.find("due:") == 0) {
            arg.arg_type = DATE;
            arg.value = token.substr(4);
        }
        else if (token.find("priority:") == 0) {
            arg.arg_type = PRIORITY;
            arg.value = token.substr(9);
        }
        else if (token.find("+") == 0) {
            arg.arg_type = TAG;
            arg.value = token.substr(1);
        }
        else {
            arg.arg_type = DESCRIPTION;
            arg.value = token;
        }

        args.push_back(arg);
    }
}

// Return the last command given to the object
string Command::cmd()
{
    string cmd_str;

    switch (action) {
        case show:
            cmd_str = "show";
        case add:
            cmd_str = "add";
        case modify:
            cmd_str = "modify";
        case done:
            cmd_str = "done";
        case del:
            cmd_str = "del";
    }

    for (Arg a : args) {
        cmd_str = "todo" + cmd_str + " " + a.value;
    }

    return cmd_str;
}