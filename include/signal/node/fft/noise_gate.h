#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{
class FFTNoiseGate : public UnaryOpNode
{
public:
    FFTNoiseGate(NodeRef input = 0, NodeRef threshold = 0.5)
        : UnaryOpNode(input)
    {
        this->name = "fft_noise_gate";
    }

    virtual void process(sample **out, int num_frames)
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
};

REGISTER(FFTNoiseGate, "fft_noise_gate");
}
