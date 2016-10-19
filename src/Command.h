#include <string>
using std::string;
#include <vector>
using std::vector;

#ifndef CMD_H
#define CMD_H

enum ArgType {VERB, DESCRIPTION, TAG, DATE, PRIORITY};
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
    // void execute();
    string cmd();
private:
    Action action;
    vector<Arg> args;
    vector<string> split_string(string str);
};

#endif