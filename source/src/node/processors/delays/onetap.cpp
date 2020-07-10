#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/delays/onetap.h"

namespace signalflow
{

OneTapDelay::OneTapDelay(NodeRef input, NodeRef delaytime, float maxdelaytime)
    : UnaryOpNode(input), delaytime(delaytime), maxdelaytime(maxdelaytime)
{
    this->name = "one-tap-delay";
    this->add_input("delay_time", this->delaytime);

    SIGNAL_CHECK_GRAPH();
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        buffers.push_back(new SampleRingBuffer(maxdelaytime * this->graph->get_sample_rate()));
    }
}

OneTapDelay::~OneTapDelay()
{
    for (auto buffer : buffers)
    {
        delete buffer;
    }
}

void OneTapDelay::process(sample **out, int num_frames)
{
    SIGNAL_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample delay = this->delaytime->out[channel][frame];
            float offset = delay * this->graph->get_sample_rate();

            out[channel][frame] = buffers[channel]->get(-offset);
            buffers[channel]->append(this->input->out[channel][frame]);
        }
    }
}

}
