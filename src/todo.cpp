#include "Command.h"

int main(int argc, char const *argv[])
{
    std::string cmd_str = "";
    for (int i = 1; i < argc; ++i) {
        cmd_str += " ";
        cmd_str += argv[i];
    }
    // Get rid of the leading space character
    cmd_str = cmd_str.substr(1);

    // Command cmd = Command(cmd_str);
    // cmd.execute();

    return 0;
}
