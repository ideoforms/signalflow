#include "signalflow/core/graph.h"
#include "signalflow/node/filters/biquad.h"

#include <stdlib.h>

/*--------------------------------------------------------------------------------*
 * Biquad filter
 * Source: https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
 **-------------------------------------------------------------------------------*/

namespace signalflow
{

BiquadFilter::BiquadFilter(NodeRef input,
                           signal_filter_type_t filter_type,
                           NodeRef cutoff,
                           NodeRef resonance,
                           NodeRef peak_gain)
    : UnaryOpNode(input), filter_type(filter_type), cutoff(cutoff), resonance(resonance), peak_gain(peak_gain)
{
    this->name = "biquad-filter";

    this->add_input("cutoff", this->cutoff);
    this->add_input("resonance", this->resonance);
    this->add_input("peak_gain", this->peak_gain);

    a0 = 1.0;
    a1 = a2 = b1 = b2 = 0.0;
    z1 = z2 = 0.0;
}

void BiquadFilter::process(sample **out, int num_frames)
{
    this->_recalculate();
    for (int channel = 0; channel < num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float in = this->input->out[channel][frame];
            float out = in * a0 + z1;
            z1 = in * a1 + z2 - b1 * out;
            z2 = in * a2 - b2 * out;
            this->out[channel][frame] = out;
        }
    }
}

void BiquadFilter::_recalculate()
{
    float norm;
    float V = powf(10.0, fabs(this->peak_gain->out[0][0]) / 20.0);
    float K = tan(M_PI * this->cutoff->out[0][0] / this->graph->get_sample_rate());
    float Q = this->resonance->out[0][0];
    float peak_gain = this->peak_gain->out[0][0];

    switch (this->filter_type)
    {
        case SIGNAL_FILTER_TYPE_LOW_PASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;

        case SIGNAL_FILTER_TYPE_HIGH_PASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = 1 * norm;
            a1 = -2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;

        case SIGNAL_FILTER_TYPE_BAND_PASS:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K / Q * norm;
            a1 = 0;
            a2 = -a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;

        case SIGNAL_FILTER_TYPE_NOTCH:
            norm = 1 / (1 + K / Q + K * K);
            a0 = (1 + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = a0;
            b1 = a1;
            b2 = (1 - K / Q + K * K) * norm;
            break;

        case SIGNAL_FILTER_TYPE_PEAK:
            if (peak_gain >= 0) // boost
            {
                norm = 1 / (1 + 1 / Q * K + K * K);
                a0 = (1 + V / Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - V / Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - 1 / Q * K + K * K) * norm;
            }
            else // cut
            {
                norm = 1 / (1 + V / Q * K + K * K);
                a0 = (1 + 1 / Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - 1 / Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - V / Q * K + K * K) * norm;
            }
            break;

        case SIGNAL_FILTER_TYPE_LOW_SHELF:
            if (peak_gain >= 0) // boost
            {
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (1 + sqrt(2 * V) * K + V * K * K) * norm;
                a1 = 2 * (V * K * K - 1) * norm;
                a2 = (1 - sqrt(2 * V) * K + V * K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else // cut
            {
                norm = 1 / (1 + sqrt(2 * V) * K + V * K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (V * K * K - 1) * norm;
                b2 = (1 - sqrt(2 * V) * K + V * K * K) * norm;
            }
            break;

        case SIGNAL_FILTER_TYPE_HIGH_SHELF:
            if (peak_gain >= 0) // boost
            {
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (V + sqrt(2 * V) * K + K * K) * norm;
                a1 = 2 * (K * K - V) * norm;
                a2 = (V - sqrt(2 * V) * K + K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else
            { // cut
                norm = 1 / (V + sqrt(2 * V) * K + K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (K * K - V) * norm;
                b2 = (V - sqrt(2 * V) * K + K * K) * norm;
            }
            break;
    }
}

}
