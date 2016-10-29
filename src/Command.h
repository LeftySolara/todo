#include "Task.h"
#include <utility>

enum Property {ID, STATUS, DESC, DUE, PRIORITY, TAG};
enum Verb {SHOW, SHOW_ALL, ADD, DONE, MODIFY, DEL, REPORT};

typedef std::pair<Property, std::string> Arg;

#ifndef COMMAND_H
#define COMMAND_H

class Command
{
public:
    Command();
    Command(std::string cmd);
    void parse(std::string cmd);
    // ~Command();
    // void execute();
private:
    std::vector<std::string> split_str(const std::string &str);
    bool is_digits(const std::string str);
    void filter_args(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end);
    // void cmd_show_all();    // TODO: Implement this
    // void cmd_show_task(int task_id);
    // void cmd_add_task(std::vector<std::string>);
    // void cmd_done_task(int task_id);
    // void cmd_delete_task(int task_id);

    std::vector<Arg> args;
    Verb verb;
};

#endif
