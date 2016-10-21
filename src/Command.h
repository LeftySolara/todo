#include <string>
using std::string;
#include <vector>
using std::vector;

#ifndef CMD_H
#define CMD_H

enum ArgType {DESCRIPTION, TAG, DATE, PRIORITY};
enum Action {SHOW, ADD, MODIFY, DONE, DEL};

struct Arg
{
    ArgType arg_type;
    string value;
};

class Command
{
public:
    Command();
    Command(string input);
    void parse(string input);
    void execute();
    void execute(string input);
    string cmd();
private:
    Action action;
    vector<Arg> args;

    void showTask();
    void addTask();
    void modifyTask();
    void doneTask();
    void deleteTask();
    
    vector<string> split_string(string str);
};

#endif
