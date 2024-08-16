#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/accumulator.h"
#include <limits>

namespace signalflow
{

Accumulator::Accumulator(NodeRef strike_force, NodeRef decay_coefficient, NodeRef trigger)
    : Node(), strike_force(strike_force), decay_coefficient(decay_coefficient), _trigger(trigger)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "accumulator";

    this->create_input("strike_force", this->strike_force);
    this->create_input("decay_coefficient", this->decay_coefficient);
    this->create_input("trigger", this->_trigger);

    if (!_trigger)
    {
        this->trigger();
    }
}

void Accumulator::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        float strike_force = value == SIGNALFLOW_NULL_FLOAT ? value : this->strike_force->out[0][0];
        float increment = strike_force * (1.0 - strike_force);
        this->current_value += increment;
    }
}

void Accumulator::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        if (SIGNALFLOW_CHECK_TRIGGER(_trigger, frame))
        {
            float strike_force = this->strike_force->out[0][frame];
            float increment = strike_force * (1.0 - this->current_value);
            this->current_value += increment;
        }
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = this->current_value;
        }
        float decay_coefficient = this->decay_coefficient->out[0][frame];
        this->current_value *= decay_coefficient;
    }
}

}
