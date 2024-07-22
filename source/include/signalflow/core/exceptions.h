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

struct insufficient_buffer_size_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    insufficient_buffer_size_exception()
        : std::runtime_error("Node cannot render because output buffer size is insufficient. Increase the buffer size. More information: https://signalflow.dev/troubleshooting/insufficient_buffer_size_exception/") {}
    insufficient_buffer_size_exception(const char *message)
        : std::runtime_error(message) {}
};

struct device_not_found_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    device_not_found_exception()
        : std::runtime_error("No such audio I/O device could be found") {}
    device_not_found_exception(const char *message)
        : std::runtime_error(message) {}
};

struct audio_io_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    audio_io_exception()
        : std::runtime_error("An audio I/O error occurred") {}
    audio_io_exception(const char *message)
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

struct cpu_usage_above_limit_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    cpu_usage_above_limit_exception()
        : std::runtime_error("Cannot play nodes as CPU usage is above specified limit") {}
    cpu_usage_above_limit_exception(const char *message)
        : std::runtime_error(message) {}
};

struct node_already_playing_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    node_already_playing_exception()
        : std::runtime_error("Node cannot be played as it is already playing. More information: https://signalflow.dev/troubleshooting/node_already_playing_exception/") {}
    node_already_playing_exception(const char *message)
        : std::runtime_error(message) {}
};

struct node_not_playing_exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;

    node_not_playing_exception()
        : std::runtime_error("Node cannot be stopped as it is not playing. More information: https://signalflow.dev/troubleshooting/node_not_playing_exception/") {}
    node_not_playing_exception(const char *message)
        : std::runtime_error(message) {}
};

}