#ifndef COMMAND_H
#define COMMAND_H

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
private:
    Action action = REPORT;
    std::vector<Arg> args;
    std::string cmd;

    std::vector<Arg> filter_args(std::vector<std::string> tokens);
};

#endif
