#pragma once

/**--------------------------------------------------------------------------------
 * @file config.h
 * @brief Application-wide configuration.
 *
 *--------------------------------------------------------------------------------*/

#include <string>

namespace signalflow
{

class SignalFlowConfig
{
public:
    SignalFlowConfig();

    /**--------------------------------------------------------------------------------
     * Get the preferred sample rate.
     * To query the actual sample rate, use AudioGraph::get_sample_rate().
     *
     * @returns The preferred sample rate, or 0 if not set.
     *
     *--------------------------------------------------------------------------------*/
    unsigned int get_sample_rate() const;

    /**--------------------------------------------------------------------------------
     * Get the preferred hardware input buffer size, in frames.
     *
     * @returns The preferred buffer size, or 0 if not set.
     *
     *--------------------------------------------------------------------------------*/
    unsigned int get_input_buffer_size() const;

    /**--------------------------------------------------------------------------------
     * Get the preferred hardware output buffer size, in frames.
     *
     * @returns The preferred buffer size, or 0 if not set.
     *
     *--------------------------------------------------------------------------------*/
    unsigned int get_output_buffer_size() const;

    /**--------------------------------------------------------------------------------
     * Get the preferred audio hardware input device name.
     *
     * @returns The name, or an empty string if not set.
     *
     *--------------------------------------------------------------------------------*/
    const std::string &get_input_device_name() const;

    /**--------------------------------------------------------------------------------
     * Get the preferred audio hardware output device name.
     *
     * @returns The name, or an empty string if not set.
     *
     *--------------------------------------------------------------------------------*/
    const std::string &get_output_device_name() const;

    /**--------------------------------------------------------------------------------
     * Print the current config to stdout.
     *
     *--------------------------------------------------------------------------------*/
    void print() const;

private:
    unsigned int sample_rate = 0;
    unsigned int input_buffer_size = 0;
    unsigned int output_buffer_size = 0;
    std::string input_device_name;
    std::string output_device_name;
};

} /* namespace signalflow */
