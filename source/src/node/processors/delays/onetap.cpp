#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/delays/onetap.h"

namespace signalflow
{

OneTapDelay::OneTapDelay(NodeRef input, NodeRef delay_time, float max_delay_time)
    : UnaryOpNode(input), delay_time(delay_time)
{
    this->name = "one-tap-delay";
    this->create_input("delay_time", this->delay_time);

    SIGNALFLOW_CHECK_GRAPH();
    for (int i = 0; i < SIGNALFLOW_MAX_CHANNELS; i++)
    {
        buffers.push_back(new SampleRingBuffer(max_delay_time * this->graph->get_sample_rate()));
    }
}

OneTapDelay::~OneTapDelay()
{
    for (auto buffer : buffers)
    {
        delete buffer;
    }
}

void OneTapDelay::process(Buffer &out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample delay = this->delay_time->out[channel][frame];
            float offset = delay * this->graph->get_sample_rate();

            /*--------------------------------------------------------------------------------
             * Append before pulling from buffer so that a delay of 0 will always pass
             * through the current frame immediately
             *-------------------------------------------------------------------------------*/
            buffers[channel]->append(this->input->out[channel][frame]);
            out[channel][frame] = buffers[channel]->get(-offset - 1);
        }
    }
}

}
