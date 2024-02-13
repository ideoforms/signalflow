#include "signalflow/core/graph.h"
#include "signalflow/node/operators/time-shift.h"
#include <math.h>

namespace signalflow
{

TimeShift::TimeShift(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "time-shift";
}

void TimeShift::process(Buffer &out, int num_frames)
{
    if (this->num_shifts_queued > 0)
    {
        printf("shifting %d blocks\n", this->num_shifts_queued);
        for (int i = 0; i < this->num_shifts_queued; i++)
        {
            this->graph->reset_subgraph(this->input);
            this->graph->render_subgraph(this->input, this->get_output_buffer_length());
        }
        this->num_shifts_queued = 0;
    }

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float value = this->input->out[channel][frame];
            out[channel][frame] = value;
        }
    }
}

void TimeShift::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        int num_blocks = (value * this->graph->get_sample_rate()) / this->get_output_buffer_length();
        this->num_shifts_queued = num_blocks;
    }
    else
    {
        this->Node::trigger(name, value);
    }
}

}
