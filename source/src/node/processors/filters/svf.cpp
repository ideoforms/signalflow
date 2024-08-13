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
    for (int frame = 0; frame < num_frames; frame++)
    {
        this->_recalculate(frame);

        for (int channel = 0; channel < num_output_channels; channel++)
        {
            float v0 = this->input->out[channel][frame];
            float v3 = v0 - ic2eq[channel];
            float v1 = a1[channel] * ic1eq[channel] + a2[channel] * v3;
            float v2 = ic2eq[channel] + a2[channel] * ic1eq[channel] + a3[channel] * v3;
            ic1eq[channel] = 2 * v1 - ic1eq[channel];
            ic2eq[channel] = 2 * v2 - ic2eq[channel];

            switch (this->filter_type->int_value())
            {
                case SIGNALFLOW_FILTER_TYPE_LOW_PASS:
                    out[channel][frame] = v2;
                    break;
                case SIGNALFLOW_FILTER_TYPE_BAND_PASS:
                    out[channel][frame] = v1;
                    break;
                case SIGNALFLOW_FILTER_TYPE_HIGH_PASS:
                    out[channel][frame] = v0 - k[channel] * v1 - v2;
                    break;
                case SIGNALFLOW_FILTER_TYPE_NOTCH:
                    out[channel][frame] = v2 + (v0 - k[channel] * v1 - v2);
                    break;
                case SIGNALFLOW_FILTER_TYPE_PEAK:
                    out[channel][frame] = v2 - (v0 - k[channel] * v1 - v2);
                    break;
                default:
                    signalflow_audio_thread_error("SVFilter: Unsupported filter type");
            }
        }
    }
}

void SVFilter::_recalculate(int frame)
{
    for (int channel = 0; channel < num_output_channels; channel++)
    {
        g[channel] = tanf(M_PI * this->cutoff->out[channel][frame] / this->graph->get_sample_rate());
        k[channel] = 2.0 - 2.0 * this->resonance->out[channel][frame];
        a1[channel] = 1 / (1 + g[channel] * (g[channel] + k[channel]));
        a2[channel] = g[channel] * a1[channel];
        a3[channel] = g[channel] * a2[channel];
    }
}

}
