#pragma once

#include "../node.h"
#include "../constants.h"
#include "../ringbuffer.h"
#include "../graph.h"

#include <vector>

namespace libsignal
{
    class Delay : public UnaryOpNode
    {
        public:
            Delay(NodeRef input = 0.0, NodeRef delaytime = 0.1, NodeRef feedback = 0.5, float maxdelaytime = 10.0);
            ~Delay();

            NodeRef delaytime;
            NodeRef feedback;
            float maxdelaytime;

            std::vector <SampleRingBuffer *> buffers;

            virtual void process(sample **out, int num_frames);
    };

    REGISTER(Delay, "delay");
}
