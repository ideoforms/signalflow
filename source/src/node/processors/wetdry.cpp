#include "signalflow/node/processors/wetdry.h"

namespace signalflow
{

WetDry::WetDry(NodeRef dry_input,
               NodeRef wet_input,
               NodeRef wetness)
    : dry_input(dry_input), wet_input(wet_input), wetness(wetness)
{
    this->name = "wetdry";

    this->create_input("dry_input", this->dry_input);
    this->create_input("wet_input", this->wet_input);
    this->create_input("wetness", this->wetness);
}

void WetDry::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = ((this->wetness->out[channel][frame] * this->wet_input->out[channel][frame])) + (((1.0 - this->wetness->out[channel][frame]) * this->dry_input->out[channel][frame]));
        }
    }
}

}
