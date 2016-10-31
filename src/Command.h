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
    void execute();
    void execute(std::string cmd);
private:
    std::vector<std::string> split_str(const std::string &str);
    bool is_digits(const std::string str);
    void filter_args(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end);
    // TODO: Implement all of these
    void cmd_show_all() { return; }
    void cmd_show_task() { return; }
    void cmd_add_task() { return; }
    void cmd_modify_task() { return; }
    void cmd_done_task() { return; }
    void cmd_delete_task() { return; }
    void cmd_report() { return; }

    std::vector<Arg> args;
    Verb verb;
};

#endif
