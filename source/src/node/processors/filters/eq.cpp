#include "signalflow/node/processors/filters/eq.h"

#include "signalflow/core/graph.h"

#include <stdlib.h>

namespace signalflow
{

EQ::EQ(NodeRef input, NodeRef low_gain, NodeRef mid_gain, NodeRef high_gain,
       NodeRef low_freq, NodeRef high_freq)
    : UnaryOpNode(input), low_gain(low_gain), mid_gain(mid_gain), high_gain(high_gain), low_freq(low_freq), high_freq(high_freq)
{
    this->name = "eq";
    this->create_input("low_gain", this->low_gain);
    this->create_input("mid_gain", this->mid_gain);
    this->create_input("high_gain", this->high_gain);
    this->create_input("low_freq", this->low_freq);
    this->create_input("high_freq", this->high_freq);

    this->alloc();
}

void EQ::alloc()
{
    this->f1p0.resize(this->num_output_channels_allocated);
    this->f1p1.resize(this->num_output_channels_allocated);
    this->f1p2.resize(this->num_output_channels_allocated);
    this->f1p3.resize(this->num_output_channels_allocated);
    this->f2p0.resize(this->num_output_channels_allocated);
    this->f2p1.resize(this->num_output_channels_allocated);
    this->f2p2.resize(this->num_output_channels_allocated);
    this->f2p3.resize(this->num_output_channels_allocated);
    this->sdm1.resize(this->num_output_channels_allocated);
    this->sdm2.resize(this->num_output_channels_allocated);
    this->sdm3.resize(this->num_output_channels_allocated);
}

void EQ::process(sample **out, int num_frames)
{
    float low, mid, high;

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float lf = 2 * sin(M_PI * ((double) this->low_freq->out[channel][frame] / this->graph->get_sample_rate()));
            float hf = 2 * sin(M_PI * ((double) this->high_freq->out[channel][frame] / this->graph->get_sample_rate()));

            /*------------------------------------------------------------------------
             * Low-pass filter
             *-----------------------------------------------------------------------*/
            float sample = this->input->out[channel][frame];
            this->f1p0[channel] += (lf * (sample - this->f1p0[channel]));
            this->f1p1[channel] += (lf * (this->f1p0[channel] - this->f1p1[channel]));
            this->f1p2[channel] += (lf * (this->f1p1[channel] - this->f1p2[channel]));
            this->f1p3[channel] += (lf * (this->f1p2[channel] - this->f1p3[channel]));
            low = this->f1p3[channel];

            /*------------------------------------------------------------------------
             * High-pass filter
             *-----------------------------------------------------------------------*/
            this->f2p0[channel] += (hf * (sample - this->f2p0[channel]));
            this->f2p1[channel] += (hf * (this->f2p0[channel] - this->f2p1[channel]));
            this->f2p2[channel] += (hf * (this->f2p1[channel] - this->f2p2[channel]));
            this->f2p3[channel] += (hf * (this->f2p2[channel] - this->f2p3[channel]));
            high = this->sdm3[channel] - this->f2p3[channel];

            /*------------------------------------------------------------------------
             * Midrange (signal - (low + high))
             *-----------------------------------------------------------------------*/
            mid = this->sdm3[channel] - (high + low);

            low *= this->low_gain->out[channel][frame];
            mid *= this->mid_gain->out[channel][frame];
            high *= this->high_gain->out[channel][frame];

            this->sdm3[channel] = this->sdm2[channel];
            this->sdm2[channel] = this->sdm1[channel];
            this->sdm1[channel] = sample;

            out[channel][frame] = low + mid + high;
        }
    }
}

}
