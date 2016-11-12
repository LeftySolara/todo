#include "Task.h"

bool operator==(const Task &lhs, const Task &rhs)
{
    return (lhs.id == rhs.id &&
            lhs.done == rhs.done && 
            lhs.description == rhs.description &&
            lhs.due_date == rhs.due_date &&
            lhs.priority == rhs.priority &&
            lhs.tags == rhs.tags);
}

bool operator!=(const Task &lhs, const Task &rhs)
{
    return (lhs.id != rhs.id ||
            lhs.done != rhs.done || 
            lhs.description != rhs.description ||
            lhs.due_date != rhs.due_date ||
            lhs.priority != rhs.priority ||
            lhs.tags != rhs.tags);
}
