#include "signalflow/node/sequencing/trigger-round-robin.h"
#include <math.h>

namespace signalflow
{

TriggerRoundRobin::TriggerRoundRobin(NodeRef direction)
    : Node(), direction(direction)
{
    this->name = "trigger-round-robin";
    this->create_input("direction", this->direction);
}

void TriggerRoundRobin::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = 0.0;
        }
    }
}

void TriggerRoundRobin::trigger(std::string name, float value)
{
    for (size_t index = 0; index < this->outputs.size(); index++)
    {
        if (index == current_output_index)
        {
            int direction = this->direction->out[0][0];
            auto output_node = this->outputs[index].first;
            output_node->trigger(name, value);
            this->current_output_index = (this->current_output_index + direction) % outputs.size();
            break;
        }
    }
}

}
