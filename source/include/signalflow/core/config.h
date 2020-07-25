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
     * Set the preferred sample rate.
     * If this is called after the AudioGraph has already been started, this will have
     * no effect until it is restarted.
     *
     * @param sample_rate The preferred sample rate, or 0 to use the default.
     *
     *--------------------------------------------------------------------------------*/
    void set_sample_rate(unsigned int sample_rate);

    /**--------------------------------------------------------------------------------
     * Get the preferred hardware input buffer size, in frames.
     *
     * @returns The preferred buffer size, or 0 if not set.
     *
     *--------------------------------------------------------------------------------*/
    unsigned int get_input_buffer_size() const;

    /**--------------------------------------------------------------------------------
     * Set the preferred input buffer size.
     * If this is called after the AudioGraph has already been started, this will have
     * no effect until it is restarted.
     *
     * @param buffer_size The preferred buffer size, or 0 to use the default.
     *
     *--------------------------------------------------------------------------------*/
    void set_input_buffer_size(unsigned int buffer_size);

    /**--------------------------------------------------------------------------------
     * Get the preferred hardware output buffer size, in frames.
     *
     * @returns The preferred buffer size, or 0 if not set.
     *
     *--------------------------------------------------------------------------------*/
    unsigned int get_output_buffer_size() const;

    /**--------------------------------------------------------------------------------
     * Set the preferred output buffer size.
     * If this is called after the AudioGraph has already been started, this will have
     * no effect until it is restarted.
     *
     * @param buffer_size The preferred buffer size, or 0 to use the default.
     *
     *--------------------------------------------------------------------------------*/
    void set_output_buffer_size(unsigned int buffer_size);

    /**--------------------------------------------------------------------------------
     * Get the name of the audio hardware device to use for input.
     *
     * @returns The name, or an empty string if not set.
     *
     *--------------------------------------------------------------------------------*/
    const std::string &get_input_device_name() const;

    /**--------------------------------------------------------------------------------
     * Set the name of the input audio device to use.
     * If this is called after the AudioGraph has already been started, this will have
     * no effect until it is restarted.
     *
     * @param name The device name.
     *
     *--------------------------------------------------------------------------------*/
    void set_input_device_name(const std::string &name);

    /**--------------------------------------------------------------------------------
     * Get the name of the audio hardware device to use for output.
     *
     * @returns The name, or an empty string if not set.
     *
     *--------------------------------------------------------------------------------*/
    const std::string &get_output_device_name() const;

    /**--------------------------------------------------------------------------------
     * Set the name of the output audio device to use.
     * If this is called after the AudioGraph has already been started, this will have
     * no effect until it is restarted.
     *
     * @param name The device name.
     *
     *--------------------------------------------------------------------------------*/
    void set_output_device_name(const std::string &name);

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
