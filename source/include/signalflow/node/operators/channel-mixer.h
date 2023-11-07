#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Downmix a multichannel input to a lower-channel output. If num_channels is
 * greater than one, spreads the input channels across the field.
 * If amplitude_compensation is enabled, scale down the amplitude based on the
 * ratio of input to output channels.
 *---------------------------------------------------------------------------------*/
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
