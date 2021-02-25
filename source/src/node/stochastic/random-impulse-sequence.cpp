#include "signalflow/node/stochastic/random-impulse-sequence.h"

namespace signalflow
{

RandomImpulseSequence::RandomImpulseSequence(NodeRef probability, NodeRef length,
                                             NodeRef clock, NodeRef explore, NodeRef generate, NodeRef reset)
    : StochasticNode(), probability(probability), length(length), clock(clock), explore(explore), generate(generate)
{
    this->name = "random-impulse-sequence";
    this->create_input("probability", this->probability);
    this->create_input("length", this->length);
    this->create_input("clock", this->clock);
    this->create_input("explore", this->explore);
    this->create_input("generate", this->generate);
    this->sequence.resize(256);

    for (size_t i = 0; i < this->sequence.size(); i++)
    {
        this->sequence[i] = std::numeric_limits<int>::max();
    }

    this->alloc();
}

void RandomImpulseSequence::alloc()
{
    this->position.resize(this->num_output_channels_allocated);
}

void RandomImpulseSequence::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            this->position[channel] = (this->position[channel] + 1) % ((int) this->length->out[channel][0]);
        }
    }
    else if (name == SIGNALFLOW_TRIGGER_GENERATE)
    {
        // Regenerate whole sequence
        for (size_t i = 0; i < this->sequence.size(); i++)
        {
            this->sequence[i] = this->random_uniform(0, 1) < this->probability->out[0][0];
        }
    }
    else if (name == SIGNALFLOW_TRIGGER_EXPLORE)
    {
        for (size_t i = 0; i < this->sequence.size(); i++)
        {
            // Toggle with small probability
            // TODO: Incorporate PExplorer algorithms (shift, swap, etc)
            if (this->random_uniform(0, 1) < 0.2)
            {
                this->sequence[i] = this->random_uniform(0, 1) < this->probability->out[0][0];
            }
        }
    }
    else
    {
        this->StochasticNode::trigger(name, value);
    }
}

void RandomImpulseSequence::process(Buffer &out, int num_frames)
{
    if (this->sequence[0] == std::numeric_limits<int>::max())
    {
        for (size_t i = 0; i < this->sequence.size(); i++)
        {
            this->sequence[i] = this->random_uniform(0, 1) < this->probability->out[0][0];
        }
    }
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->explore, channel, frame))
            {
                this->trigger(SIGNALFLOW_TRIGGER_EXPLORE);
            }
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->generate, channel, frame))
            {
                this->trigger(SIGNALFLOW_TRIGGER_GENERATE);
            }

            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame))
            {
                this->out[channel][frame] = this->sequence[this->position[channel]];
                this->position[channel] = (this->position[channel] + 1) % ((int) this->length->out[channel][frame]);
            }
            else
            {
                this->out[channel][frame] = 0.0;
            }
        }
    }
}

}
