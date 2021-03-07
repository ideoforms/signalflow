#include "signalflow/node/sequencing/impulse-sequence.h"

namespace signalflow
{

ImpulseSequence::ImpulseSequence(std::vector<int> sequence, NodeRef clock)
    : Node(), sequence(sequence), clock(clock)
{
    this->name = "impulse-sequence";
    this->create_input("clock", this->clock);
    this->alloc();
}

ImpulseSequence::ImpulseSequence(std::string sequence, NodeRef clock)
    : ImpulseSequence(signalflow_binary_sequence_to_vector(sequence), clock)
{
}

void ImpulseSequence::alloc()
{
    this->position.resize(this->num_output_channels_allocated);
}

void ImpulseSequence::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int i = 0; i < this->num_output_channels; i++)
        {
            this->position[i] = (this->position[i] + 1) % sequence.size();
        }
    }
}

void ImpulseSequence::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool rv = SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
            if (rv)
            {
                out[channel][frame] = this->sequence[this->position[channel]];
                this->position[channel] = (this->position[channel] + 1) % sequence.size();
            }
            else
            {
                out[channel][frame] = 0.0;
            }
        }
    }
}

}
