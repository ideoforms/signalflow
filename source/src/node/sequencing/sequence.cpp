#include "signalflow/node/sequencing/sequence.h"

#define SIGNALFLOW_SEQUENCE_NOT_YET_STARTED -1

namespace signalflow
{

Sequence::Sequence(std::vector<float> sequence, NodeRef clock)
    : Node(), sequence(sequence), clock(clock)
{
    this->name = "sequence";
    this->create_input("clock", this->clock);
    this->alloc();
}

void Sequence::alloc()
{
    /*--------------------------------------------------------------------------------
     * Until the sequence has been triggered for the first time, use a special
     * position value to denote that zero should be returned.
     *--------------------------------------------------------------------------------*/
    this->position.resize(this->num_output_channels_allocated,
                          SIGNALFLOW_SEQUENCE_NOT_YET_STARTED);
}

void Sequence::trigger(std::string name, float value)
{
    for (int i = 0; i < this->num_output_channels; i++)
    {
        this->position[i] = (this->position[i] + 1) % sequence.size();
    }
}

void Sequence::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool rv = SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
            if (rv)
            {
                this->position[channel] = (this->position[channel] + 1) % sequence.size();
            }

            if (this->position[channel] == SIGNALFLOW_SEQUENCE_NOT_YET_STARTED)
            {
                this->out[channel][frame] = 0.0;
            }
            else
            {
                this->out[channel][frame] = this->sequence[this->position[channel]];
            }
        }
    }
}

}
