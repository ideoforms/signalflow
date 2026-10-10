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
    int sample_rate = this->graph->get_sample_rate();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        /*--------------------------------------------------------------------------------
         * Keep steps_remaining in a local variable during the loop, to avoid a load
         * and store per sample (see SineOscillator).
         *--------------------------------------------------------------------------------*/
        float steps_remaining = this->steps_remaining[channel];
        sample *frequency_channel = this->frequency->out[channel];
        sample *out_channel = out[channel];

        for (int frame = 0; frame < num_frames; frame++)
        {
            sample rv = 0;
            if (steps_remaining <= 0)
            {
                rv = 1;
                float freq_in = frequency_channel[frame];
                if (freq_in > 0)
                {
                    /*--------------------------------------------------------------------------------
                     * Add the float number of samples, rather than simply setting `steps_remaining`,
                     * to ensure we don't accumulate rounding-down errors when Fs/freq is not
                     * an integer (consider the case in which Fs = 44100 and freq = 8: samples
                     * per cycle would be 5512.5, which would be rounded down to 5512.)
                     *-------------------------------------------------------------------------------*/
                    steps_remaining += sample_rate / freq_in;
                }
                else
                {
                    steps_remaining = INT_MAX;
                }
            }

            steps_remaining--;

            out_channel[frame] = rv;
        }
        this->steps_remaining[channel] = steps_remaining;
    }
}

void Impulse::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            this->steps_remaining[channel] = 0;
        }
    }
}

}
