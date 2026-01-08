#include "signalflow/core/core.h"
#include "signalflow/node/operators/bus.h"
#include "signalflow/node/oscillators/constant.h"

namespace signalflow
{

Bus::Bus(unsigned int num_channels)
    : VariableInputNode()
{
    this->name = "bus";

    /*------------------------------------------------------------------------
     * Must include false here to avoid breaking multichannel buses.
     * Without it, inputs are not automatically upmixed.
     *------------------------------------------------------------------------*/
    this->set_channels(num_channels, num_channels, false);
}

void Bus::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        memset(this->out[channel], 0, sizeof(sample) * num_frames);
        for (NodeRef input : this->input_list)
        {
#ifdef __APPLE__
            vDSP_vadd(input->out[channel], 1, out[channel], 1, out[channel], 1, num_frames);
#else
            for (int frame = 0; frame < num_frames; frame++)
            {
                out[channel][frame] += input->out[channel][frame];
            }
#endif
        }
    }
}

}
