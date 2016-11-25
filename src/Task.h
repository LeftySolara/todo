#include <string>
#include <vector>
#include <ostream>

#ifndef TASK_H
#define TASK_H

enum Priority {none, low, med, high};

struct Task
{
    int id;
    bool done;
    std::string description;
    std::string due_date;
    Priority priority;
    std::vector<std::string> tags;
};

bool operator==(const Task &lhs, const Task &rhs);
bool operator!=(const Task &lhs, const Task &rhs);
std::ostream& operator<<(std::ostream &os, const Task &task);

#endif
