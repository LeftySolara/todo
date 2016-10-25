#include <string>
#include <vector>

#ifndef TASK_H
#define TASK_H

enum Priority {low, med, high};

struct Task
{
    const int id;
    bool done;
    std::string description;
    std::string due_date;
    Priority priority;
    std::vector<std::string> tags;
};

#endif