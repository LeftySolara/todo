#include <stdexcept>

namespace exceptions
{
    class database_error;
    // class script_error;   
};

class exceptions::database_error : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not read database file";
    }
} db_error;