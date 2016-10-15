#include <iostream>
using std::cout; using std::cin; using std::endl;
#include <string>
using std::string;
#include "commands.h"
#include "task.h"

int Task::nextId = 0;

int main(int argc, char const *argv[])
{
    Task task1 = Task("First");
    Task task2 = Task("Second");
    Task task3 = Task("Third");

    string cmd = argv[1];
    if (cmd == "add")
        CmdAdd();
    else if (cmd == "remove")
        CmdRemove();
    else if (cmd == "done")
        CmdDone();
    else if (cmd == "show") {
        cout << task1 << endl;
        cout << task2 << endl;
        cout << task3 << endl;
    }
    else if (cmd == "modify")
        CmdModify();
    else
        cout << "Unknown command" << endl;
}
