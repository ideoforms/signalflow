#pragma once 

#include "signal/node.h"

namespace libsignal
{
    class TriggerNoise : public Node
    {
    public:
        TriggerNoise(NodeRef min = 0.0, NodeRef max = 1.0, NodeRef clock = 0.0);

        NodeRef min;
        NodeRef max;
        NodeRef clock;

        sample value[SIGNAL_MAX_CHANNELS];

        virtual void process(sample **out, int num_frames);
        virtual void trigger(std::string = SIGNAL_DEFAULT_TRIGGER, float value = 0.0);

    };

    REGISTER(TriggerNoise, "trigger_noise");
}
