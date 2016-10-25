#include "Task.h"
#include <utility>

enum Property {id, status, desc, due, priority, tag};

typedef std::pair<Property, std::string> Arg;

#ifndef COMMAND_H
#define COMMAND_H

class Command
{
public:
    Command();
    // Command(std::string cmd);
    // ~Command();
    // void execute();
    // void parse();
    // void parse(std::string cmd);
private:
    std::vector<Arg> args;
};

#endif