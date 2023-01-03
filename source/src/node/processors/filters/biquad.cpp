#include "signalflow/core/graph.h"
#include "signalflow/node/processors/filters/biquad.h"

#include <stdlib.h>

/*--------------------------------------------------------------------------------*
 * Biquad filter
 * Source: https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
 **-------------------------------------------------------------------------------*/

namespace signalflow
{

BiquadFilter::BiquadFilter(NodeRef input,
                           signalflow_filter_type_t filter_type,
                           NodeRef cutoff,
                           NodeRef resonance,
                           NodeRef peak_gain)
    : UnaryOpNode(input), filter_type(filter_type), cutoff(cutoff), resonance(resonance), peak_gain(peak_gain)
{
    this->name = "biquad-filter";

    this->create_input("cutoff", this->cutoff);
    this->create_input("resonance", this->resonance);
    this->create_input("peak_gain", this->peak_gain);

    this->alloc();
}

void BiquadFilter::alloc()
{
    this->a0.resize(this->num_output_channels_allocated, 1.0);
    this->a1.resize(this->num_output_channels_allocated, 0.0);
    this->a2.resize(this->num_output_channels_allocated, 0.0);
    this->b1.resize(this->num_output_channels_allocated, 0.0);
    this->b2.resize(this->num_output_channels_allocated, 0.0);
    this->z1.resize(this->num_output_channels_allocated, 0.0);
    this->z2.resize(this->num_output_channels_allocated, 0.0);
}

void BiquadFilter::process(Buffer &out, int num_frames)
{
    this->_recalculate();
    for (int channel = 0; channel < num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float in = this->input->out[channel][frame];
            float value = in * a0[channel] + z1[channel];
            z1[channel] = in * a1[channel] + z2[channel] - b1[channel] * value;
            z2[channel] = in * a2[channel] - b2[channel] * value;
            out[channel][frame] = value;
        }
    }
}

void BiquadFilter::_recalculate()
{
    for (int channel = 0; channel < num_output_channels; channel++)
    {
        float norm;
        float V = powf(10.0, fabs(this->peak_gain->out[channel][0]) / 20.0);
        float K = tan(M_PI * this->cutoff->out[channel][0] / this->graph->get_sample_rate());
        /*--------------------------------------------------------------------------------
         * Ensure Q does not go to 0.0 to avoid divide-by-zero errors.
         *--------------------------------------------------------------------------------*/
        float Q = this->resonance->out[channel][0];
        Q = MAX(Q, 1e-9);
        float peak_gain = this->peak_gain->out[channel][0];

        switch (this->filter_type)
        {
            case SIGNALFLOW_FILTER_TYPE_LOW_PASS:
                norm = 1 / (1 + K / Q + K * K);
                a0[channel] = K * K * norm;
                a1[channel] = 2 * a0[channel];
                a2[channel] = a0[channel];
                b1[channel] = 2 * (K * K - 1) * norm;
                b2[channel] = (1 - K / Q + K * K) * norm;
                break;

            case SIGNALFLOW_FILTER_TYPE_HIGH_PASS:
                norm = 1 / (1 + K / Q + K * K);
                a0[channel] = 1 * norm;
                a1[channel] = -2 * a0[channel];
                a2[channel] = a0[channel];
                b1[channel] = 2 * (K * K - 1) * norm;
                b2[channel] = (1 - K / Q + K * K) * norm;
                break;

            case SIGNALFLOW_FILTER_TYPE_BAND_PASS:
                norm = 1 / (1 + K / Q + K * K);
                a0[channel] = K / Q * norm;
                a1[channel] = 0;
                a2[channel] = -a0[channel];
                b1[channel] = 2 * (K * K - 1) * norm;
                b2[channel] = (1 - K / Q + K * K) * norm;
                break;

            case SIGNALFLOW_FILTER_TYPE_NOTCH:
                norm = 1 / (1 + K / Q + K * K);
                a0[channel] = (1 + K * K) * norm;
                a1[channel] = 2 * (K * K - 1) * norm;
                a2[channel] = a0[channel];
                b1[channel] = a1[channel];
                b2[channel] = (1 - K / Q + K * K) * norm;
                break;

            case SIGNALFLOW_FILTER_TYPE_PEAK:
                if (peak_gain >= 0) // boost
                {
                    norm = 1 / (1 + 1 / Q * K + K * K);
                    a0[channel] = (1 + V / Q * K + K * K) * norm;
                    a1[channel] = 2 * (K * K - 1) * norm;
                    a2[channel] = (1 - V / Q * K + K * K) * norm;
                    b1[channel] = a1[channel];
                    b2[channel] = (1 - 1 / Q * K + K * K) * norm;
                }
                else // cut
                {
                    norm = 1 / (1 + V / Q * K + K * K);
                    a0[channel] = (1 + 1 / Q * K + K * K) * norm;
                    a1[channel] = 2 * (K * K - 1) * norm;
                    a2[channel] = (1 - 1 / Q * K + K * K) * norm;
                    b1[channel] = a1[channel];
                    b2[channel] = (1 - V / Q * K + K * K) * norm;
                }
                break;

            case SIGNALFLOW_FILTER_TYPE_LOW_SHELF:
                if (peak_gain >= 0) // boost
                {
                    norm = 1 / (1 + sqrt(2) * K + K * K);
                    a0[channel] = (1 + sqrt(2 * V) * K + V * K * K) * norm;
                    a1[channel] = 2 * (V * K * K - 1) * norm;
                    a2[channel] = (1 - sqrt(2 * V) * K + V * K * K) * norm;
                    b1[channel] = 2 * (K * K - 1) * norm;
                    b2[channel] = (1 - sqrt(2) * K + K * K) * norm;
                }
                else // cut
                {
                    norm = 1 / (1 + sqrt(2 * V) * K + V * K * K);
                    a0[channel] = (1 + sqrt(2) * K + K * K) * norm;
                    a1[channel] = 2 * (K * K - 1) * norm;
                    a2[channel] = (1 - sqrt(2) * K + K * K) * norm;
                    b1[channel] = 2 * (V * K * K - 1) * norm;
                    b2[channel] = (1 - sqrt(2 * V) * K + V * K * K) * norm;
                }
                break;

            case SIGNALFLOW_FILTER_TYPE_HIGH_SHELF:
                if (peak_gain >= 0) // boost
                {
                    norm = 1 / (1 + sqrt(2) * K + K * K);
                    a0[channel] = (V + sqrt(2 * V) * K + K * K) * norm;
                    a1[channel] = 2 * (K * K - V) * norm;
                    a2[channel] = (V - sqrt(2 * V) * K + K * K) * norm;
                    b1[channel] = 2 * (K * K - 1) * norm;
                    b2[channel] = (1 - sqrt(2) * K + K * K) * norm;
                }
                else
                { // cut
                    norm = 1 / (V + sqrt(2 * V) * K + K * K);
                    a0[channel] = (1 + sqrt(2) * K + K * K) * norm;
                    a1[channel] = 2 * (K * K - 1) * norm;
                    a2[channel] = (1 - sqrt(2) * K + K * K) * norm;
                    b1[channel] = 2 * (K * K - V) * norm;
                    b2[channel] = (V - sqrt(2 * V) * K + K * K) * norm;
                }
                break;

            default:
                throw std::runtime_error("Invalid filter type");
        }
    }
}

}
