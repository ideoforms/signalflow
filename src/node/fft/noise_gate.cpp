#ifdef __APPLE__

#include "signal/node/fft/noise_gate.h"

namespace libsignal
{

FFTNoiseGate::FFTNoiseGate(NodeRef input, NodeRef threshold)
    : UnaryOpNode(input)
{
    this->name = "fft_noise_gate";
}

void FFTNoiseGate::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < 4; channel++)
    {
        for (int i = 0; i < num_frames; i++)
        {
            // Magnitudes
            if (i < num_frames / 2)
            {
                float rv = input->out[channel][i];
                printf("%f\n", rv);
                out[channel][i] = (rv < 20) ? rv : 0;
            }
            // Phases
            else
            {
                out[channel][i] = input->out[channel][i];
            }
        }
    }
}

}


#endif