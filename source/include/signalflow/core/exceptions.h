#pragma once

#include <stdexcept>

namespace signalflow
{

struct graph_not_created_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    graph_not_created_exception()
        : std::runtime_error("No AudioGraph has been created") {}
    graph_not_created_exception(const char *message)
        : std::runtime_error(message) {}
};

}