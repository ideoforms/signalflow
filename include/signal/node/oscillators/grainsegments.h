#pragma once

#include "signal/node/node.h"
#include "signal/buffer/buffer.h"
#include "granulator.h"

#include <vector>

namespace libsignal
{
    class GrainSegments : public Node
    {
        public:
            GrainSegments(BufferRef buffer = nullptr,
                          NodeRef clock = 0,
                          NodeRef target = 0,
                          PropertyRef offsets = { },
                          PropertyRef values = { },
                          PropertyRef durations = { });

            BufferRef buffer;
            BufferRef envelope;

            NodeRef clock;
            NodeRef target;
            PropertyRef offsets;
            PropertyRef values;
            PropertyRef durations;

            virtual void process(sample **out, int num_frames);

        private:
            sample clock_last;

            std::vector <Grain *> grains;
    };

    REGISTER(GrainSegments, "grain_segments");
}
