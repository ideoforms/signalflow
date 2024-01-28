#include "signalflow/node/fft/fft-flip-spectrum.h"

namespace signalflow
{

FFTFlipSpectrum::FFTFlipSpectrum(NodeRef input, NodeRef flip, NodeRef rotate)
    : FFTOpNode(input), flip(flip), rotate(rotate)
{
    this->name = "fft-flip-spectrum";

    this->create_input("flip", this->flip);
    this->create_input("rotate", this->rotate);
}

void FFTFlipSpectrum::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                bool flip = this->flip->out[0][frame];
                int rotate = (int) this->rotate->out[0][frame];
                int frame_after_transform = frame;
                if (flip)
                {
                    frame_after_transform = this->num_bins - 1 - frame_after_transform;
                }
                frame_after_transform = (frame_after_transform - rotate) % this->num_bins;
                if (frame_after_transform < 0)
                {
                    frame_after_transform += this->num_bins;
                }

                out[hop][frame] = input->out[hop][frame_after_transform];
            }
            else
            {
                out[hop][frame] = input->out[hop][frame];
            }
        }
    }
}

}
