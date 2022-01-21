#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class AzimuthPanner : public Node
{
public:
    AzimuthPanner(int channels = 2, NodeRef input = 0, NodeRef pan = 0.0, NodeRef width = 1.0);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void set_property(std::string property_name, const PropertyRef &value) override;

    NodeRef input;
    NodeRef pan;
    NodeRef width;
    PropertyRef num_channels;
};

REGISTER(AzimuthPanner, "azimuth-panner")
}
