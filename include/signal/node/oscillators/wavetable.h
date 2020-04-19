#pragma once 

#include "signal/node/node.h"

namespace libsignal
{
    class Wavetable : public Node
    {
    public:
        Wavetable(BufferRef table = nullptr, NodeRef frequency = 440);

        virtual void process(sample **out, int num_frames);
    private:
        BufferRef table;
        NodeRef frequency;
        float phase;

    };

    class Wavetable2D : public Node
    {
    public:
        Wavetable2D(BufferRef2D buffer = nullptr, NodeRef frequency = 440, NodeRef crossfade = 0.0);

        virtual void process(sample **out, int num_frames);
    private:
        BufferRef2D buffer;
        NodeRef frequency;
        NodeRef crossfade;
        float phase;

    };

    REGISTER(Wavetable, "wavetable");
    REGISTER(Wavetable2D, "wavetable2D");
}
