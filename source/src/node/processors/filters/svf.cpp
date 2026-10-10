#include "signalflow/core/graph.h"
#include "signalflow/node/processors/filters/svf.h"

#include <math.h>

/*--------------------------------------------------------------------------------*
 * State variable filter
 * Source: https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
 *
 * Python code:

    data, fs = sf.read("examples/audio/amen-brother-mono.wav")

    def svf(ar, fs):
        ic1eq = 0
        ic2eq = 0
        g = math.tan(math.pi * 1000 / fs)
        k = 2 - 2 * 0.9
        a1 = 1 / (1 + g * (g + k))
        a2 = g * a1
        a3 = g * a2

        lpf = np.zeros(len(ar))
        bpf = np.zeros(len(ar))
        hpf = np.zeros(len(ar))
        notch = np.zeros(len(ar))
        peak = np.zeros(len(ar))

        for index, v0 in enumerate(ar):
            v3 = v0 - ic2eq
            v1 = a1 * ic1eq + a2 * v3
            v2 = ic2eq + a2 * ic1eq + a3 * v3
            ic1eq = 2 * v1 - ic1eq
            ic2eq = 2 * v2 - ic2eq
            lpf[index] = v2
            bpf[index] = v1
            hpf[index] = v0 - k * v1 - v2
            notch[index] = lpf[index] + hpf[index]
            peak[index] = lpf[index] - hpf[index]
        return lpf, bpf, hpf, notch, peak

    lpf, bpf, hpf, notch, peak = svf(data, fs)

 **-------------------------------------------------------------------------------*/

namespace signalflow
{

SVFilter::SVFilter(NodeRef input,
                   signalflow_filter_type_t filter_type,
                   NodeRef cutoff,
                   NodeRef resonance)
    : UnaryOpNode(input), filter_type((int) filter_type), cutoff(cutoff), resonance(resonance)
{
    this->name = "sv-filter";

    this->create_property("filter_type", this->filter_type);
    this->create_input("cutoff", this->cutoff);
    this->create_input("resonance", this->resonance);

    this->alloc();
}

SVFilter::SVFilter(NodeRef input,
                   std::string filter_type,
                   NodeRef cutoff,
                   NodeRef resonance)
    : SVFilter(input, SIGNALFLOW_FILTER_TYPE_MAP[filter_type], cutoff, resonance)
{
}

void SVFilter::alloc()
{
    this->ic1eq.resize(this->num_output_channels_allocated, 0.0);
    this->ic2eq.resize(this->num_output_channels_allocated, 0.0);
    this->g.resize(this->num_output_channels_allocated, 0.0);
    this->k.resize(this->num_output_channels_allocated, 0.0);
    this->a1.resize(this->num_output_channels_allocated, 0.0);
    this->a2.resize(this->num_output_channels_allocated, 0.0);
    this->a3.resize(this->num_output_channels_allocated, 0.0);
}

void SVFilter::process(Buffer &out, int num_frames)
{
    // Cache filter_type rather than querying property each iteration, for efficiency
    signalflow_filter_type_t filter_type = (signalflow_filter_type_t) this->filter_type->int_value();
    float fs = this->graph->get_sample_rate();
    float nyquist = fs / 2;

    for (int channel = 0; channel < num_output_channels; channel++)
    {
        /*--------------------------------------------------------------------------------
         * Keep coefficients and filter state in local variables during the loop.
         * Accessing the member vectors directly forces loads and stores per sample,
         * as the compiler cannot rule out that they alias the output buffer.
         *--------------------------------------------------------------------------------*/
        float c_ic1eq = ic1eq[channel], c_ic2eq = ic2eq[channel];
        float c_g = g[channel], c_k = k[channel];
        float c_a1 = a1[channel], c_a2 = a2[channel], c_a3 = a3[channel];
        sample *in_channel = this->input->out[channel];
        sample *cutoff_channel = this->cutoff->out[channel];
        sample *resonance_channel = this->resonance->out[channel];
        sample *out_channel = out[channel];

        /*--------------------------------------------------------------------------------
         * Coefficients are only recalculated when cutoff or resonance change, as
         * tanf() is expensive and these are frequently constant. NAN ensures that
         * coefficients are always calculated on the first frame of each block.
         *--------------------------------------------------------------------------------*/
        float last_cutoff = NAN;
        float last_resonance = NAN;

        for (int frame = 0; frame < num_frames; frame++)
        {
            float cutoff = cutoff_channel[frame];
            float resonance = resonance_channel[frame];
            if (cutoff != last_cutoff || resonance != last_resonance)
            {
                last_cutoff = cutoff;
                last_resonance = resonance;
                cutoff = fmin(cutoff, nyquist);
                c_g = tanf(M_PI * cutoff / fs);
                c_k = 2.0 - 2.0 * resonance;
                c_a1 = 1 / (1 + c_g * (c_g + c_k));
                c_a2 = c_g * c_a1;
                c_a3 = c_g * c_a2;
            }

            float v0 = in_channel[frame];
            float v3 = v0 - c_ic2eq;
            float v1 = c_a1 * c_ic1eq + c_a2 * v3;
            float v2 = c_ic2eq + c_a2 * c_ic1eq + c_a3 * v3;
            c_ic1eq = 2 * v1 - c_ic1eq;
            c_ic2eq = 2 * v2 - c_ic2eq;

            switch (filter_type)
            {
                case SIGNALFLOW_FILTER_TYPE_LOW_PASS:
                    out_channel[frame] = v2;
                    break;
                case SIGNALFLOW_FILTER_TYPE_BAND_PASS:
                    out_channel[frame] = v1;
                    break;
                case SIGNALFLOW_FILTER_TYPE_HIGH_PASS:
                    out_channel[frame] = v0 - c_k * v1 - v2;
                    break;
                case SIGNALFLOW_FILTER_TYPE_NOTCH:
                    out_channel[frame] = v2 + (v0 - c_k * v1 - v2);
                    break;
                case SIGNALFLOW_FILTER_TYPE_PEAK:
                    out_channel[frame] = v2 - (v0 - c_k * v1 - v2);
                    break;
                default:
                    signalflow_audio_thread_error("SVFilter: Unsupported filter type");
            }
        }

        ic1eq[channel] = c_ic1eq;
        ic2eq[channel] = c_ic2eq;
        g[channel] = c_g;
        k[channel] = c_k;
        a1[channel] = c_a1;
        a2[channel] = c_a2;
        a3[channel] = c_a3;
    }
}

}
