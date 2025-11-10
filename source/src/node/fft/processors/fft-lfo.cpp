#include "signalflow/core/graph.h"
#include "signalflow/node/fft/processors/fft-lfo.h"

namespace signalflow
{

FFTLFO::FFTLFO(NodeRef input, NodeRef frequency, NodeRef spectral_cycles)
    : FFTOpNode(input), frequency(frequency), spectral_cycles(spectral_cycles)
{
    this->name = "fft-contrast";

    this->create_input("frequency", this->frequency);
    this->create_input("spectral_cycles", this->spectral_cycles);
    this->phase = 0.0;
}

void FFTLFO::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;
    // TODO: Could this update per hop if I index based on hop?
    float increment_per_bin = this->spectral_cycles->out[0][0] / (M_PI * 2);

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        float frequency = this->frequency->out[0][0];

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                float phase_accum = phase + (increment_per_bin * frame / num_frames);
                out[hop][frame] = input->out[hop][frame] * (sinf(M_PI * 2 * phase_accum) * 2 + 1.0);
            }
            else
            {
                out[hop][frame] = input->out[hop][frame];
            }

            phase += frequency / this->graph->get_sample_rate();
            while (phase > 1.0)
            {
                phase -= 1.0;
            }
        }
    }
}

}
