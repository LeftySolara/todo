#include <stdexcept>

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

namespace exceptions
{
    class database_error;
    class sql_error;   
};

class exceptions::database_error : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not read database file";
    }
} db_error;

class exceptions::sql_error : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Error executing SQL command";
    }
} sql_error;

#endif
