#include "signalflow/node/fft/processors/fft-contrast.h"
#include <algorithm>

namespace signalflow
{

FFTContrast::FFTContrast(NodeRef input, NodeRef contrast)
    : FFTOpNode(input), contrast(contrast)
{
    this->name = "fft-contrast";

    this->create_input("contrast", this->contrast);
}

void FFTContrast::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        memcpy(this->mags, &input->out[hop][0], sizeof(float) * num_bins);
        std::sort(this->mags, mags + num_bins);
        float average_magnitude = this->mags[(int) (this->num_bins / 2)];
        float min_magnitude = this->mags[0];
        float max_magnitude = this->mags[this->num_bins - 1];
        //        float average_magnitude = signalflow_array_mean(&this->out[hop][0], num_bins);

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            float contrast = this->contrast->out[0][frame];
            if (frame < this->num_bins)
            {
                float input_value = this->input->out[hop][frame];
                float input_delta_from_average = input_value - average_magnitude;
                float output_value = average_magnitude + (contrast * input_delta_from_average);
                output_value = signalflow_clip(output_value, min_magnitude, max_magnitude);

                out[hop][frame] = output_value;
            }
            else
            {
                out[hop][frame] = input->out[hop][frame];
            }
        }
    }
}

}
