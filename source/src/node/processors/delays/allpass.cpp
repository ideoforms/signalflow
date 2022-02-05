#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/delays/allpass.h"

#include <stdlib.h>

namespace signalflow
{

AllpassDelay::AllpassDelay(NodeRef input, NodeRef delay_time, NodeRef feedback, float max_delay_time)
    : UnaryOpNode(input), delay_time(delay_time), feedback(feedback)
{
    this->name = "allpass-delay";
    this->create_input("delay_time", this->delay_time);
    this->create_input("feedback", this->feedback);

    SIGNALFLOW_CHECK_GRAPH();
    for (int i = 0; i < SIGNALFLOW_MAX_CHANNELS; i++)
    {
        buffers.push_back(new SampleRingBuffer(max_delay_time * this->graph->get_sample_rate()));
    }
}

AllpassDelay::~AllpassDelay()
{
    for (auto buffer : buffers)
    {
        delete buffer;
    }
}

void AllpassDelay::process(Buffer &out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample delay = this->delay_time->out[channel][frame];
            sample feedback = this->feedback->out[channel][frame];
            float offset = delay * this->graph->get_sample_rate();

            // Moorer combined feedforward/feedback comb filter, giving an allpass response
            // https://valhalladsp.com/2011/01/21/reverbs-diffusion-allpass-delays-and-metallic-artifacts/
            sample v = input->out[channel][frame] - feedback * buffers[channel]->get(-offset);
            sample rv = feedback * v + buffers[channel]->get(-offset);
            out[channel][frame] = rv;
            buffers[channel]->append(v);
        }
    }
}

}
