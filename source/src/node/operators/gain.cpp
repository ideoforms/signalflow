#include "signalflow/core/util.h"
#include "signalflow/node/operators/gain.h"

namespace signalflow
{

Gain::Gain(NodeRef input, NodeRef gain)
    : UnaryOpNode(input), gain(gain)
{
    this->name = "gain";

    this->create_input("gain", this->gain);
}

void Gain::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample gain_db = this->gain->out[channel][frame];
            if (gain_db != this->last_gain_db)
            {
                this->last_gain_db = gain_db;
                this->last_gain_amplitude = signalflow_db_to_amplitude(gain_db);
            }
            out[channel][frame] = this->input->out[channel][frame] * this->last_gain_amplitude;
        }
    }
}

}
