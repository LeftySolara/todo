#ifndef COMMAND_H
#define COMMAND_H

#include "Database.h"

#include <string>
#include <vector>
#include <utility>

class Command
{
public:
    enum Action {SHOW, SHOW_ALL, ADD, DONE, MODIFY, DEL, REPORT};
    enum Property {ID, STATUS, DESC, DUE, PRIORITY, TAG};
    typedef std::pair<Property, std::string> Arg;

    Command();
    Command(std::string cmd_str);
    void parse(std::string cmd_str);
    void execute();
private:
    Action action = REPORT;
    std::vector<Arg> args;
    std::string cmd;
    Database db;

    void filter_args(const std::vector<std::string> &tokens);
    void cmd_show_task() { return; }
    void cmd_show_all() { return; }
    void cmd_add_task();
    void cmd_done_task() { return; }
    void cmd_modify_task() { return; }
    void cmd_delete_task() { return; }
    void cmd_report() { return; }
};

#endif
