#pragma once 

#include "signal/node.h"

namespace libsignal
{
    class Wavetable : public Node
    {
    public:
        Wavetable(BufferRef table = nullptr, NodeRef frequency = 440, bool interpolate = true);

        virtual void process(sample **out, int num_frames);

        BufferRef table;
        NodeRef frequency;
        float phase;
        bool interpolate;

    };

    REGISTER(Wavetable, "wavetable");
}
