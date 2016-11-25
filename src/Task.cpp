#include "Task.h"
#include "utils.h"

#include <iostream>

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

std::ostream& operator<<(std::ostream &os, const Task &task)
{
    std::string status = "pending";
    if (task.done) {
        status = "done";
    }

    std::string priority;
    if (task.priority == 1) {
        priority = "low";
    }
    else if (task.priority == 2) {
        priority = "medium";
    }
    else if (task.priority == 3) {
        priority = "high";
    }
    else {
        priority = "none";
    }

    std::string tags = utils::join(task.tags);

    int left_width = 13;
    int right_width = task.description.length();

    // Header
    os.width(left_width); os << std::left << "Name";
    os.width(right_width); os << std::left << "Value" << std::endl;
    os.width(left_width); os.fill('-'); os << std::right << " ";
    os.width(right_width); os.fill('-'); os << std::right << "" << std::endl;

    os.fill(' '); os << std::left << "";
    os.width(left_width); os << std::left<< "ID" << task.id << std::endl;
    os.width(left_width); os << std::left<< "Status" << status << std::endl;
    os.width(left_width); os << std::left<< "Description" << task.description << std::endl;
    os.width(left_width); os << std::left<< "Due" << task.due_date << std::endl;
    os.width(left_width); os << std::left<< "Priority" << priority << std::endl;
    os.width(left_width); os << std::left<< "Tags" << tags << std::endl;

    return os;
}