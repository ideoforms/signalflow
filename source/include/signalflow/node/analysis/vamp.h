#ifdef HAVE_VAMP

#pragma once

#include "signalflow/node/node.h"

#include <vamp-hostsdk/PluginHostAdapter.h>
#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginLoader.h>

using Vamp::Plugin;

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Feature extraction using the Vamp plugin toolkit.
 *---------------------------------------------------------------------------------*/
class VampAnalysis : public UnaryOpNode
{
public:
    VampAnalysis(NodeRef input = 0.0, std::string plugin_id = "vamp-example-plugins:spectralcentroid:linearcentroid");
    ~VampAnalysis();

    virtual void process(Buffer &out, int num_frames);
    std::vector<std::string> list_plugins();

protected:
    int current_frame;
    int output_index;
    Plugin *plugin;
};

class VampEventExtractor : public VampAnalysis
{
public:
    VampEventExtractor(NodeRef input = 0.0, std::string plugin_id = "vamp:vamp-example-plugins:percussiononsets:onsets");
    virtual void process(Buffer &out, int num_frames);

    PropertyRef timestamps;
    PropertyRef labels;
};

class VampSegmenter : public VampAnalysis
{
public:
    VampSegmenter(NodeRef input = 0.0, std::string plugin_id = "vamp:vamp-example-plugins:percussiononsets:onsets");
    virtual void process(Buffer &out, int num_frames);

private:
    float last_value = -1;
    long last_timestamp = -1;
};

REGISTER(VampAnalysis, "vamp")
REGISTER(VampEventExtractor, "vamp-events")
REGISTER(VampSegmenter, "vamp-segmenter")

}

#endif