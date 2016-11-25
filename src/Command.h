#ifndef COMMAND_H
#define COMMAND_H

#include "Database.h"

#include <string>
#include <vector>
#include <utility>

class Command
{
public:
    enum Action {SHOW, ADD, DONE, MODIFY, DEL, REPORT, CLEAR};
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
    int get_id();
    int cmd_show_task();
    int cmd_add_task();
    int cmd_done_task();
    int cmd_modify_task();
    int cmd_delete_task();
    int cmd_clear();
    int cmd_report() { return 0; }
};

#endif
