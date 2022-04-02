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

struct graph_already_created_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    graph_already_created_exception()
        : std::runtime_error("AudioGraph has already been created") {}
    graph_already_created_exception(const char *message)
        : std::runtime_error(message) {}
};

struct invalid_channel_count_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    invalid_channel_count_exception()
        : std::runtime_error("Invalid channel count") {}
    invalid_channel_count_exception(const char *message)
        : std::runtime_error(message) {}
};

struct patch_finished_playback_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    patch_finished_playback_exception()
        : std::runtime_error("Cannot play Patch as it has already finished playback") {}
    patch_finished_playback_exception(const char *message)
        : std::runtime_error(message) {}
};

}