#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

class ChannelMixer : public UnaryOpNode
{

public:
    ChannelMixer(int channels = 1,
                 NodeRef input = 0,
                 bool amplitude_compensation = true);

    virtual void process(Buffer &out, int num_frames);
    virtual void update_channels();

    int channels;
    bool amplitude_compensation;
    float amplitude_compensation_level;
};

REGISTER(ChannelMixer, "channel-mixer")

}
