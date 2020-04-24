#include "signal/node/filters/delay.h"
#include "signal/node/oscillators/constant.h"

#include <stdlib.h>

namespace libsignal
{

Delay::Delay(NodeRef input, NodeRef delaytime, NodeRef feedback, float maxdelaytime)
    : UnaryOpNode(input), delaytime(delaytime), feedback(feedback), maxdelaytime(maxdelaytime)
{
    this->name = "delay";
    this->add_input("delay_time", this->delaytime);
    this->add_input("feedback", this->feedback);

    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        buffers.push_back(new SampleRingBuffer(maxdelaytime * this->graph->sample_rate));
    }
}

Delay::~Delay()
{
    // free buffers
}

void Delay::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample delay = this->delaytime->out[channel][frame];
            sample feedback = this->feedback->out[channel][frame];
            float offset = delay * this->graph->sample_rate;

            sample rv = buffers[channel]->get(-offset);
            out[channel][frame] = rv;
            buffers[channel]->append(input->out[channel][frame] + (feedback * rv));
        }
    }
}

}
