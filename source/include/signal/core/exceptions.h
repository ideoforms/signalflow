#pragma once

#include <stdexcept>

namespace libsignal
{

struct graph_not_created_exception : public std::exception
{
    // graph_not_created_exception() : std::exception() {}

    const char *what () const throw ()
    {
        return "No AudioGraph has been created";
    }
};

}