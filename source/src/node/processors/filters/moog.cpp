#include "signalflow/core/graph.h"
#include "signalflow/node/processors/filters/moog.h"

#include <stdlib.h>

namespace signalflow
{

MoogVCF::MoogVCF(NodeRef input, NodeRef cutoff, NodeRef resonance)
    : UnaryOpNode(input), cutoff(cutoff), resonance(resonance)
{
    this->name = "moog";
    this->create_input("cutoff", this->cutoff);
    this->create_input("resonance", this->resonance);

    this->alloc();
}

void MoogVCF::alloc()
{
    this->out1.resize(this->num_output_channels_allocated);
    this->out2.resize(this->num_output_channels_allocated);
    this->out3.resize(this->num_output_channels_allocated);
    this->out4.resize(this->num_output_channels_allocated);
    this->in1.resize(this->num_output_channels_allocated);
    this->in2.resize(this->num_output_channels_allocated);
    this->in3.resize(this->num_output_channels_allocated);
    this->in4.resize(this->num_output_channels_allocated);
}

void MoogVCF::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float cutoff = signalflow_scale_lin_lin(this->cutoff->out[channel][frame], 0, this->graph->get_sample_rate() / 2, 0.005, 1);

            /*------------------------------------------------------------------------
             * The original algorithm typically expects resonance values between
             * [0, 4]. Rescale in this implementation, for consistency with other
             * filter nodes that expect [0, 1].
             *-----------------------------------------------------------------------*/
            float resonance = this->resonance->out[channel][frame] * 4.0;
            float f = cutoff * 1.16;
            float fb = resonance * (1.0 - 0.15 * f * f);

            /*------------------------------------------------------------------------
             * Calculate filter
             *-----------------------------------------------------------------------*/
            float input = this->input->out[channel][frame];

            input -= out4[channel] * fb;
            input *= 0.35013 * f * f * f * f;

            /*------------------------------------------------------------------------
             * Pole 1
             *-----------------------------------------------------------------------*/
            out1[channel] = input + 0.3 * in1[channel] + (1 - f) * out1[channel];
            in1[channel] = input;

            /*------------------------------------------------------------------------
             * Pole 2
             *-----------------------------------------------------------------------*/
            out2[channel] = out1[channel] + 0.3 * in2[channel] + (1 - f) * out2[channel];
            in2[channel] = out1[channel];

            /*------------------------------------------------------------------------
             * Pole 3
             *-----------------------------------------------------------------------*/
            out3[channel] = out2[channel] + 0.3 * in3[channel] + (1 - f) * out3[channel];
            in3[channel] = out2[channel];

            /*------------------------------------------------------------------------
             * Pole 4
             *-----------------------------------------------------------------------*/
            out4[channel] = out3[channel] + 0.3 * in4[channel] + (1 - f) * out4[channel];
            in4[channel] = out3[channel];

            out[channel][frame] = out4[channel];
        }
    }
}

}
