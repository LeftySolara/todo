#ifndef TASK_H
#define TASK_H

#include <ctime>
#include <string>
using std::string;
#include <iostream>
using std::ostream;

enum Priority {NONE, LOW, MED, HIGH};

class Task
{
public:
    Task(string desc, time_t due_date = 0, Priority priority = NONE);
    static int nextId;
    friend ostream& operator<<(ostream& os, const Task& task);
protected:
    int id;
    string description;
    time_t due_date;
    Priority priority;
};

ostream& operator<<(ostream& os, const Task& task);

#endif //TASK_H
