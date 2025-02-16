#include "signalflow/core/config.h"
#include "signalflow/core/constants.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <stdlib.h>

namespace signalflow
{

std::map<std::string, std::map<std::string, std::string>> parse_config(std::istream &stream)
{
    std::string line;
    std::map<std::string, std::map<std::string, std::string>> sections;

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
    std::string config_path = SIGNALFLOW_USER_DIR + "/config";
    std::ifstream input(config_path);

    /*--------------------------------------------------------------------------------
     * Don't throw an error if the user config file does not exist,
     * just continue silently.
     *--------------------------------------------------------------------------------*/
    if (input.good())
    {
        parse_file(input);
    }

    parse_env();
}

AudioGraphConfig::AudioGraphConfig(std::string config_path)
{
    std::ifstream input(config_path);
    /*--------------------------------------------------------------------------------
     * For user-specified paths, try opening the unqualified path initially,
     * then relative to the user dir.
     *--------------------------------------------------------------------------------*/
    if (!input.good())
    {
        input.open(SIGNALFLOW_USER_DIR + "/" + config_path);
        if (!input.good())
        {
            throw std::runtime_error("Config path could not be found: " + config_path);
        }
    }

    parse_file(input);
    parse_env();
}

void AudioGraphConfig::parse_file(std::ifstream &input)
{
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
                else if (parameter_name == "backend_name")
                {
                    this->backend_name = parameter_value;
                }
                else if (parameter_name == "cpu_usage_limit")
                {
                    this->cpu_usage_limit = std::stof(parameter_value);
                }
                else if (parameter_name == "auto_record")
                {
                    std::vector<std::string> true_values = { "yes", "on", "true", "1" };
                    std::vector<std::string> false_values = { "no", "off", "false", "0" };
                    if (std::find(true_values.begin(), true_values.end(), parameter_value) != true_values.end())
                    {
                        this->auto_record = true;
                    }
                    else if (std::find(false_values.begin(), false_values.end(), parameter_value) != false_values.end())
                    {
                        this->auto_record = false;
                    }
                    else
                    {
                        throw std::runtime_error("Invalid value for auto_record: " + parameter_value);
                    }
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
}

void AudioGraphConfig::parse_env()
{
    if (getenv("SIGNALFLOW_SAMPLE_RATE"))
    {
        this->sample_rate = atoi(getenv("SIGNALFLOW_SAMPLE_RATE"));
    }
    if (getenv("SIGNALFLOW_INPUT_BUFFER_SIZE"))
    {
        this->input_buffer_size = atoi(getenv("SIGNALFLOW_INPUT_BUFFER_SIZE"));
    }
    if (getenv("SIGNALFLOW_OUTPUT_BUFFER_SIZE"))
    {
        this->output_buffer_size = atoi(getenv("SIGNALFLOW_OUTPUT_BUFFER_SIZE"));
    }
    if (getenv("SIGNALFLOW_INPUT_DEVICE_NAME"))
    {
        this->input_device_name = std::string(getenv("SIGNALFLOW_INPUT_DEVICE_NAME"));
    }
    if (getenv("SIGNALFLOW_BACKEND_NAME"))
    {
        this->backend_name = std::string(getenv("SIGNALFLOW_BACKEND_NAME"));
    }
    if (getenv("SIGNALFLOW_OUTPUT_DEVICE_NAME"))
    {
        this->output_device_name = std::string(getenv("SIGNALFLOW_OUTPUT_DEVICE_NAME"));
    }
    if (getenv("SIGNALFLOW_CPU_USAGE_LIMIT"))
    {
        this->cpu_usage_limit = atof(getenv("SIGNALFLOW_CPU_USAGE_LIMIT"));
    }
    if (getenv("SIGNALFLOW_AUTO_RECORD"))
    {
        this->auto_record = (bool) atoi(getenv("SIGNALFLOW_AUTO_RECORD"));
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

const std::string &AudioGraphConfig::get_backend_name() const { return this->backend_name; }

void AudioGraphConfig::set_backend_name(const std::string &name) { this->backend_name = name; }

float AudioGraphConfig::get_cpu_usage_limit() const { return this->cpu_usage_limit; }

void AudioGraphConfig::set_cpu_usage_limit(float limit) { this->cpu_usage_limit = limit; }

bool AudioGraphConfig::get_auto_record() const { return this->auto_record; }

void AudioGraphConfig::set_auto_record(bool on) { this->auto_record = on; }

void AudioGraphConfig::print() const
{
    std::cout << "backend_name = " << this->backend_name << std::endl;
    std::cout << "output_device_name = " << this->output_device_name << std::endl;
    std::cout << "output_buffer_size = " << this->output_buffer_size << std::endl;
    std::cout << "input_device_name = " << this->input_device_name << std::endl;
    std::cout << "input_buffer_size = " << this->input_buffer_size << std::endl;
    std::cout << "sample_rate = " << this->sample_rate << std::endl;
    std::cout << "cpu_usage_limit = " << this->cpu_usage_limit << std::endl;
}

}
