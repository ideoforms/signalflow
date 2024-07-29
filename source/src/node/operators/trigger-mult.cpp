#include "signalflow/node/operators/trigger-mult.h"
#include <math.h>

namespace signalflow
{

TriggerMult::TriggerMult(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "trigger-mult";
}

void TriggerMult::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float value = this->input->out[channel][frame];
            out[channel][frame] = value;
        }
    }
}

void TriggerMult::trigger(std::string name, float value)
{
    for (auto output : this->get_outputs())
    {
        auto output_node = output.first;
        output_node->trigger(name, value);
    }
}

}
