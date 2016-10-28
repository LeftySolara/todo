#include "Task.h"
#include <utility>

enum Property {id, status, desc, due, priority, tag};
enum Verb {show, add, done, modify, del};

typedef std::pair<Property, std::string> Arg;

#ifndef COMMAND_H
#define COMMAND_H

class Command
{
public:
    Command();
    Command(const std::string &cmd);
    // ~Command();
    // void execute();
    void parse(std::string &cmd="");
    // void parse(std::string cmd);
private:
    std::vector<std::string> split_str(const std::string &str);
    bool is_digits(cost::string &str)
    std::vector<Arg> args;
    Verb verb;
};

#endif