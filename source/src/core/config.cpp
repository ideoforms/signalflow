#include "signalflow/core/config.h"
#include "signalflow/core/constants.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <stdlib.h>
#include <unordered_map>

namespace signalflow
{

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> parse_config(std::istream &stream)
{
    std::string line;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sections;

    std::string current_section;

    const std::regex section_regex(R"x(\s*\[([^\]]+)\]\s*)x");
    const std::regex config_regex(R"x(\s*([a-z0-9_-]+)\s*=\s*("?)(\S[^"]*)\2\s*)x");
    const std::regex comment_regex(R"x(\s*#.*)x");
    const std::regex other_content_regex(R"x(\S)x");

    unsigned int line_number = 1;
    while (std::getline(stream, line))
    {
        std::smatch match;

        if (std::regex_match(line, match, section_regex))
        {
            const std::string &section_name = match[1];
            if (sections.find(section_name) != sections.end())
            {
                throw std::runtime_error("Found repeated section name on line " + std::to_string(line_number) + ": "
                                         + section_name);
            }
            current_section = section_name;
        }
        else if (std::regex_match(line, match, config_regex))
        {
            const std::string &item_name = match[1];
            const std::string &item_value = match[3];

            if (current_section.empty())
            {
                throw std::runtime_error("Found invalid config item without a parent section on line "
                                         + std::to_string(line_number) + ": " + item_name);
            }
            if (item_value == "null")
            {
                sections[current_section][item_name] = "";
            }
            else
            {
                sections[current_section][item_name] = item_value;
            }
        }
        else if (std::regex_search(line, match, comment_regex))
        {
        }
        else if (std::regex_search(line, match, other_content_regex))
        {
            throw std::runtime_error("Invalid config string on line " + std::to_string(line_number) + ": " + line);
        }
        line_number++;
    }

    return sections;
}

AudioGraphConfig::AudioGraphConfig()
{
    const std::string config_path = SIGNALFLOW_USER_DIR + "/config";

    std::ifstream input(config_path);
    if (!input.good())
    {
        return;
    }

    auto sections = parse_config(input);
    for (auto section_pair : sections)
    {
        auto section_name = section_pair.first;
        auto section_values = section_pair.second;
        for (auto parameter_pair : section_values)
        {
            auto parameter_name = parameter_pair.first;
            auto parameter_value = parameter_pair.second;

            if (section_name == "audio")
            {
                if (parameter_name == "sample_rate")
                {
                    this->sample_rate = std::stoi(parameter_value);
                }
                else if (parameter_name == "input_buffer_size")
                {
                    this->input_buffer_size = std::stoi(parameter_value);
                }
                else if (parameter_name == "output_buffer_size")
                {
                    this->output_buffer_size = std::stoi(parameter_value);
                }
                else if (parameter_name == "input_device_name")
                {
                    this->input_device_name = parameter_value;
                }
                else if (parameter_name == "output_device_name")
                {
                    this->output_device_name = parameter_value;
                }
                else if (parameter_name == "output_backend_name")
                {
                    this->output_backend_name = parameter_value;
                }
                else if (parameter_name == "cpu_usage_limit")
                {
                    this->cpu_usage_limit = std::stof(parameter_value);
                }
                else
                {
                    throw std::runtime_error("Invalid section parameter name: " + section_name + " > "
                                             + parameter_name);
                }
            }
            else if (section_name == "midi")
            {
                /*--------------------------------------------------------------------------------
                 * midi config stanza is used by the signalflow_midi Python module.
                 * Ignore it.
                 *--------------------------------------------------------------------------------*/
            }
            else
            {
                throw std::runtime_error("Invalid section name: " + section_name);
            }
        }
    }

    if (getenv("SIGNALFLOW_SAMPLE_RATE"))
    {
        this->sample_rate = atoi(getenv("SIGNALFLOW_SAMPLE_RATE"));
    }
    if (getenv("SIGNALFLOW_INPUT_BUFFER_SIZE"))
    {
        this->output_buffer_size = atoi(getenv("SIGNALFLOW_INPUT_BUFFER_SIZE"));
    }
    if (getenv("SIGNALFLOW_OUTPUT_BUFFER_SIZE"))
    {
        this->output_buffer_size = atoi(getenv("SIGNALFLOW_OUTPUT_BUFFER_SIZE"));
    }
    if (getenv("SIGNALFLOW_INPUT_DEVICE_NAME"))
    {
        this->output_device_name = std::string(getenv("SIGNALFLOW_INPUT_DEVICE_NAME"));
    }
    if (getenv("SIGNALFLOW_OUTPUT_BACKEND_NAME"))
    {
        this->output_backend_name = std::string(getenv("SIGNALFLOW_OUTPUT_BACKEND_NAME"));
    }
    if (getenv("SIGNALFLOW_OUTPUT_DEVICE_NAME"))
    {
        this->output_device_name = std::string(getenv("SIGNALFLOW_OUTPUT_DEVICE_NAME"));
    }
    if (getenv("SIGNALFLOW_CPU_USAGE_LIMIT"))
    {
        this->cpu_usage_limit = atof(getenv("SIGNALFLOW_CPU_USAGE_LIMIT"));
    }
}

unsigned int AudioGraphConfig::get_sample_rate() const { return this->sample_rate; }

void AudioGraphConfig::set_sample_rate(unsigned int sample_rate) { this->sample_rate = sample_rate; }

unsigned int AudioGraphConfig::get_input_buffer_size() const { return this->input_buffer_size; }

void AudioGraphConfig::set_input_buffer_size(unsigned int buffer_size) { this->input_buffer_size = buffer_size; }

unsigned int AudioGraphConfig::get_output_buffer_size() const { return this->output_buffer_size; }

void AudioGraphConfig::set_output_buffer_size(unsigned int buffer_size) { this->output_buffer_size = buffer_size; }

const std::string &AudioGraphConfig::get_input_device_name() const { return this->input_device_name; }

void AudioGraphConfig::set_input_device_name(const std::string &name) { this->input_device_name = name; }

const std::string &AudioGraphConfig::get_output_device_name() const { return this->output_device_name; }

void AudioGraphConfig::set_output_device_name(const std::string &name) { this->output_device_name = name; }

const std::string &AudioGraphConfig::get_output_backend_name() const { return this->output_backend_name; }

void AudioGraphConfig::set_output_backend_name(const std::string &name) { this->output_backend_name = name; }

float AudioGraphConfig::get_cpu_usage_limit() const { return this->cpu_usage_limit; }

void AudioGraphConfig::set_cpu_usage_limit(float limit) { this->cpu_usage_limit = limit; }

void AudioGraphConfig::print() const
{
    std::cout << "SignalFlow config" << std::endl;
    std::cout << "-----------------" << std::endl;
    std::cout << " - sample_rate = " << this->sample_rate << std::endl;
    std::cout << " - input_buffer_size = " << this->input_buffer_size << std::endl;
    std::cout << " - output_buffer_size = " << this->output_buffer_size << std::endl;
    std::cout << " - input_device_name = " << this->input_device_name << std::endl;
    std::cout << " - output_device_name = " << this->output_device_name << std::endl;
    std::cout << " - cpu_usage_limit = " << this->cpu_usage_limit << std::endl;
}

}
