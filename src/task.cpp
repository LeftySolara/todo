#include "task.h"

Task::Task(string desc, time_t due, Priority pri)
{
    id = ++nextId;
    description = desc;
    due_date = due;
    priority = pri;
}

ostream& operator<<(ostream& os, const Task& task)
{
    os << "ID: " << task.id << '\n'
       << "Description: " << task.description << '\n'
       << "Due: " << task.due_date << '\n'
       << "Priority: " << task.priority << '\n';
    return os;
}

int Task::nextId = 0;