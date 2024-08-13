#include "signalflow/core/core.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/delays/comb.h"

#include <stdlib.h>

namespace signalflow
{

CombDelay::CombDelay(NodeRef input, NodeRef delay_time, NodeRef feedback, float max_delay_time)
    : UnaryOpNode(input), delay_time(delay_time), feedback(feedback), max_delay_time(max_delay_time)
{
    this->name = "comb-delay";
    this->create_input("delay_time", this->delay_time);
    this->create_input("feedback", this->feedback);

    SIGNALFLOW_CHECK_GRAPH();
    for (int i = 0; i < SIGNALFLOW_MAX_CHANNELS; i++)
    {
        buffers.push_back(new SampleRingBuffer(max_delay_time * this->graph->get_sample_rate()));
    }
}

CombDelay::~CombDelay()
{
    for (auto buffer : buffers)
    {
        delete buffer;
    }
}

void CombDelay::process(Buffer &out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample delay_time = this->delay_time->out[channel][frame];
            sample feedback = this->feedback->out[channel][frame];
            float offset = delay_time * this->graph->get_sample_rate();

            if (delay_time > this->max_delay_time)
            {
                signalflow_audio_thread_error("CombDelay: Delay time exceeds maximum. Reduce the delay_time, or increase max_delay_time.");
            }

            sample rv = input->out[channel][frame] + (feedback * buffers[channel]->get(-offset));
            out[channel][frame] = rv;
            buffers[channel]->append(rv);
        }
    }
}

}
