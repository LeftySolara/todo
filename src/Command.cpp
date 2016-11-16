#include <sstream>
#include "Command.h"

Command::Command()
{
    cmd = "";
    parse(cmd);
}

Command::Command(std::string cmd_str)
{
    cmd = cmd_str;
    parse(cmd);
}

void Command::parse(std::string cmd_str)
{
    std::vector<std::string> args = split(cmd_str);
    std::string action = args.front();
}

std::vector<std::string> Command::split(const std::string &str, char delim)
{
    std::string token;
    std::stringstream stream;
    std::vector<std::string> tokens;
    size_t pos = 0;

    stream.str(str);
    while (std::getline(stream, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}
