#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/delays/comb.h"

#include <stdlib.h>

namespace signalflow
{

CombDelay::CombDelay(NodeRef input, NodeRef delaytime, NodeRef feedback, float maxdelaytime)
    : UnaryOpNode(input), delaytime(delaytime), feedback(feedback)
{
    this->name = "comb-delay";
    this->create_input("delay_time", this->delaytime);
    this->create_input("feedback", this->feedback);

    SIGNALFLOW_CHECK_GRAPH();
    for (int i = 0; i < SIGNALFLOW_MAX_CHANNELS; i++)
    {
        buffers.push_back(new SampleRingBuffer(maxdelaytime * this->graph->get_sample_rate()));
    }
}

CombDelay::~CombDelay()
{
    for (auto buffer : buffers)
    {
        delete buffer;
    }
}

void CombDelay::process(sample **out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample delay = this->delaytime->out[channel][frame];
            sample feedback = this->feedback->out[channel][frame];
            float offset = delay * this->graph->get_sample_rate();

            sample rv = input->out[channel][frame] + (feedback * buffers[channel]->get(-offset));
            out[channel][frame] = rv;
            buffers[channel]->append(rv);
        }
    }
}

}
