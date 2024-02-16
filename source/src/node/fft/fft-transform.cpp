#include "signalflow/node/fft/fft-transform.h"

namespace signalflow
{

FFTTransform::FFTTransform(NodeRef input, NodeRef flip, NodeRef rotate)
    : FFTOpNode(input), flip(flip), rotate(rotate)
{
    this->name = "fft-transform";

    this->create_input("flip", this->flip);
    this->create_input("rotate", this->rotate);
}

void FFTTransform::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        // One of the gotchas of FFT nodes is that they can't be modulated at audio
        // rate as they process at a different rate (in FFT-size'd blocks of samples)
        // so just take the first value of the flip/rotate inputs.
        bool flip = this->flip->out[0][0];
        int rotate = (int) this->rotate->out[0][0];

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                int frame_after_transform = frame - rotate;
                if (flip)
                {
                    frame_after_transform = this->num_bins - 1 - frame_after_transform;
                }
                frame_after_transform = frame_after_transform % this->num_bins;
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
