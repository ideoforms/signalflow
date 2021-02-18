#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/impulse.h"
#include <limits.h>

namespace signalflow
{

Impulse::Impulse(NodeRef frequency)
    : frequency(frequency)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "impulse";
    this->create_input("frequency", this->frequency);

    this->alloc();
}

void Impulse::alloc()
{
    this->steps_remaining.resize(this->num_output_channels_allocated);
}

void Impulse::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample rv = 0;
            if (this->steps_remaining[channel] <= 0)
            {
                rv = 1;
                float freq_in = this->frequency->out[channel][frame];
                if (freq_in > 0)
                {
                    /*--------------------------------------------------------------------------------
                     * Add the float number of samples, rather than simply setting `steps_remaining`,
                     * to ensure we don't accumulate rounding-down errors when Fs/freq is not
                     * an integer (consider the case in which Fs = 44100 and freq = 8: samples
                     * per cycle would be 5512.5, which would be rounded down to 5512.)
                     *-------------------------------------------------------------------------------*/
                    this->steps_remaining[channel] += this->graph->get_sample_rate() / this->frequency->out[channel][frame];
                }
                else
                {
                    this->steps_remaining[channel] = INT_MAX;
                }
            }

            this->steps_remaining[channel]--;

            out[channel][frame] = rv;
        }
    }
}

}
