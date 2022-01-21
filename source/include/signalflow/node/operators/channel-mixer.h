#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

class ChannelMixer : public UnaryOpNode
{

public:
    ChannelMixer(int num_channels = 1,
                 NodeRef input = 0,
                 bool amplitude_compensation = true);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void set_property(std::string name, const PropertyRef &value) override;
    virtual void update_channels() override;

    PropertyRef num_channels;
    bool amplitude_compensation;
    float amplitude_compensation_level;
};

REGISTER(ChannelMixer, "channel-mixer")

}
