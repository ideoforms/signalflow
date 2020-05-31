#include "signal/node/fft/zero_phase.h"

namespace libsignal
{

FFTZeroPhase::FFTZeroPhase(NodeRef input)
    : UnaryOpNode(input)
{
    this->name = "zero_phase";
}

void FFTZeroPhase::process(sample **out, int num_frames)
{
    for (int i = 0; i < num_frames; i++)
    {
        if (i > 0 && i < num_frames / 2)
        {
            if (i < num_frames / 4)
                out[0][i] = input->out[0][i];
            else
                out[0][i] = input->out[0][i];
        }
        else
            out[0][i] = input->out[0][i] * 0.1;
    }
}

}
