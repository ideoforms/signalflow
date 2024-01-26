#ifdef HAVE_VAMP

#include "signalflow/core/core.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/analysis/vamp.h"

#include <algorithm>

using Vamp::RealTime;
using Vamp::HostExt::PluginLoader;

namespace signalflow
{

std::vector<std::string> VampAnalysis::list_plugins()
{
    PluginLoader *loader = PluginLoader::getInstance();
    std::vector<std::string> plugins = loader->listPlugins();
    return plugins;
}

VampAnalysis::VampAnalysis(NodeRef input, std::string plugin_id)
    : UnaryOpNode(input)
{
    this->name = "vamp";
    this->current_frame = 0;

    /*------------------------------------------------------------------------
     * Strip leading `vamp:` from plugin ID, if given.
     *-----------------------------------------------------------------------*/
    if (plugin_id.find("vamp:") == 0)
        plugin_id = plugin_id.substr(5);

    /*------------------------------------------------------------------------
     * Check that plugin ID is valid.
     *-----------------------------------------------------------------------*/
    size_t num_colons = std::count(plugin_id.begin(), plugin_id.end(), ':');
    if (num_colons != 2)
        throw std::runtime_error("Invalid Vamp plugin ID: " + plugin_id);

    int first_separator = plugin_id.find(':');
    int second_separator = plugin_id.find(':', first_separator + 1);
    std::string vamp_plugin_library = plugin_id.substr(0, first_separator);
    std::string vamp_plugin_feature = plugin_id.substr(first_separator + 1, second_separator - first_separator - 1);
    std::string vamp_plugin_output = plugin_id.substr(second_separator + 1);
    signalflow_debug("Loading plugin %s, %s, %s", vamp_plugin_library.c_str(), vamp_plugin_feature.c_str(), vamp_plugin_output.c_str());

    PluginLoader *loader = PluginLoader::getInstance();
    PluginLoader::PluginKey key = loader->composePluginKey(vamp_plugin_library, vamp_plugin_feature);

    this->plugin = loader->loadPlugin(key, this->graph->get_sample_rate(), PluginLoader::ADAPT_ALL);

    if (!this->plugin)
    {
        std::cerr << "WARNING: Couldn't find Vamp plugin: " << plugin_id << std::endl;
        std::cerr << "Information on finding and installing Vamp plugins: https://www.vamp-plugins.org/download.html" << std::endl;
    }
    else
    {
        /*------------------------------------------------------------------------
         * Get required output index.
         *-----------------------------------------------------------------------*/
        Plugin::OutputList outputs = this->plugin->getOutputDescriptors();
        this->output_index = -1;

        for (unsigned int oi = 0; oi < outputs.size(); oi++)
        {
            if (outputs[oi].identifier == vamp_plugin_output)
            {
                this->output_index = oi;
                break;
            }
        }

        signalflow_debug("Loaded plugin (output index %d)", this->output_index);
        bool rv = this->plugin->initialise(1, graph->get_output_buffer_size(), graph->get_output_buffer_size());
        // bool rv = this->plugin->initialise(1, 1024, 1024);
        if (!rv)
        {
            printf("WARNING: Vamp initialisation reported failure\n");
        }
    }
}

VampAnalysis::~VampAnalysis()
{
    // close vamp plugins
}

void VampAnalysis::process(Buffer &out, int num_frames)
{
    if (!this->plugin)
    {
        return;
    }

    RealTime rt = RealTime::frame2RealTime(this->current_frame, this->graph->get_sample_rate());
    Plugin::FeatureSet features = this->plugin->process(this->input->out.data, rt);
    if (features[this->output_index].size())
    {
        Plugin::Feature feature = features[this->output_index][0];

        if (feature.values.size() > 0)
        {
            float value = feature.values[0];

            for (int channel = 0; channel < this->num_input_channels; channel++)
            {
                for (int frame = 0; frame < num_frames; frame++)
                {
                    out[channel][frame] = value;
                }
            }

            this->current_frame += num_frames;
        }
        else
        {
            for (int channel = 0; channel < this->num_input_channels; channel++)
            {
                memset(out[channel], 0, num_frames * sizeof(sample));
            }
        }
    }
    else
    {
        // no features extracted
    }
}

VampEventExtractor::VampEventExtractor(NodeRef input, std::string plugin_id)
    : VampAnalysis(input, plugin_id)
{
    this->name = "vamp_events";

    this->create_property("timestamps", this->timestamps);
    this->create_property("labels", this->labels);

    this->set_property("timestamps", { 0 });
    this->set_property("labels", { "" });
}

void VampEventExtractor::process(Buffer &out, int num_frames)
{
    RealTime rt = RealTime::frame2RealTime(this->current_frame, this->graph->get_sample_rate());
    Plugin::FeatureSet features = this->plugin->process(this->input->out.data, rt);
    if (features[this->output_index].size())
    {
        Plugin::Feature feature = features[this->output_index][0];

        if (feature.hasTimestamp)
        {
            long ts = RealTime::realTime2Frame(feature.timestamp, this->graph->get_sample_rate());
            std::vector<float> timestamps = this->get_property("timestamps")->float_array_value();
            timestamps.push_back(ts);
            this->set_property("timestamps", timestamps);
        }
    }
}

VampSegmenter::VampSegmenter(NodeRef input, std::string plugin_id)
    : VampAnalysis(input, plugin_id)
{
    this->name = "vamp_segmenter";
    this->set_property("timestamps", { 0 });
    this->set_property("values", { 0 });
    this->set_property("durations", { 0 });
}

void VampSegmenter::process(Buffer &out, int num_frames)
{
    RealTime rt = RealTime::frame2RealTime(this->current_frame, this->graph->get_sample_rate());
    Plugin::FeatureSet features = this->plugin->process(this->input->out.data, rt);
    if (features[this->output_index].size())
    {
        Plugin::Feature feature = features[this->output_index][0];

        if (feature.values.size() > 0)
        {
            long timestamp = RealTime::realTime2Frame(feature.timestamp, this->graph->get_sample_rate());
            float value = feature.values[0];
            value = signalflow_midi_note_to_frequency(roundf(signalflow_frequency_to_midi_note(value)));

            if (value != last_value && (!isnan(value) || !isnan(last_value)))
            {
                if (!isnan(value))
                {
                    std::vector<float> values = this->get_property("values")->float_array_value();
                    values.push_back(value);
                    this->set_property("values", values);

                    std::vector<float> timestamps = this->get_property("timestamps")->float_array_value();
                    timestamps.push_back(timestamp);
                    this->set_property("timestamps", timestamps);
                }

                if (last_timestamp >= 0 && !isnan(last_value))
                {
                    float duration = (float) (timestamp - last_timestamp);
                    std::vector<float> durations = this->get_property("durations")->float_array_value();
                    durations.push_back(duration);
                    this->set_property("durations", durations);
                }

                this->last_value = value;
                this->last_timestamp = timestamp;
            }
        }
    }
}

}

#endif