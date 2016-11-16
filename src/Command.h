#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class Command
{
public:
    Command();
    Command(std::string cmd_str);
    void parse(std::string cmd_str);
private:
    std::vector<std::string> split(const std::string &str, char delim=' ');
    std::string cmd;
};

#endif
