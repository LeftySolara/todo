#ifndef COMMAND_H
#define COMMAND_H

#include "Database.h"

#include <string>
#include <vector>
#include <utility>

class Command
{
public:
    enum Action {SHOW, SHOW_ALL, ADD, DONE, MODIFY, DEL, REPORT, CLEAR};
    enum Property {ID, STATUS, DESC, DUE, PRIORITY, TAG};
    typedef std::pair<Property, std::string> Arg;

    Command();
    Command(std::string cmd_str);
    void parse(std::string cmd_str);
    int execute();
private:
    Action action = REPORT;
    std::vector<Arg> args;
    std::string cmd;

    void filter_args(const std::vector<std::string> &tokens);
    int cmd_show_task();
    int cmd_show_all() { return 0; }
    int cmd_add_task();
    int cmd_done_task() { return 0; }
    int cmd_modify_task() { return 0; }
    int cmd_delete_task() { return 0; }
    int cmd_clear();
    int cmd_report() { return 0; }
};

#endif
