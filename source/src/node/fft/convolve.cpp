#ifdef __APPLE__

#include "signal/node/fft/convolve.h"
#include "signal/node/fft/fft.h"

namespace libsignal
{

FFTConvolve::FFTConvolve(NodeRef input, BufferRef buffer)
    : FFTOpNode(input), buffer(buffer)
{
    this->name = "fft-convolve";

    this->num_partitions = ceil((buffer->num_frames - this->fft_size) / this->hop_size) + 1;
    this->ir_partitions.resize(this->num_partitions);
    this->input_history.resize(this->num_partitions);

    printf("Buffer length %d frames, fft size %d, hop size %d, doing %d partitions\n",
        buffer->num_frames, this->fft_size, this->hop_size, this->num_partitions);

    FFT *fft = new FFT();
    for (int i = 0; i < this->num_partitions; i++)
    {
        this->ir_partitions[i] = new sample[this->fft_size]();
        this->input_history[i] = new sample[this->fft_size]();
        fft->fft(this->buffer->data[0] + i * this->hop_size, this->ir_partitions[i]);
    }
    delete fft;

    this->add_buffer("buffer", this->buffer);
}

FFTConvolve::~FFTConvolve()
{
    for (auto partition : this->ir_partitions)
    {
        delete partition;
    }
}

void FFTConvolve::process(sample **out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Iterate over fft_size frames as each block contains a whole
         * fft of samples.
         *-----------------------------------------------------------------------*/
        for (int frame = 0; frame < this->fft_size; frame++)
        {
                if (frame < this->num_bins)
                {
                    /*------------------------------------------------------------------------
                     * Magnitudes
                     *-----------------------------------------------------------------------*/
                    this->out[hop][frame] = this->input->out[hop][frame] * this->ir_partitions[0][frame];
                }
                else
                {
                    /*------------------------------------------------------------------------
                     * Phases
                     *-----------------------------------------------------------------------*/
                    this->out[hop][frame] = this->input->out[hop][frame] + this->ir_partitions[0][frame];
                }
        }

        /*------------------------------------------------------------------------
         * Each hop, roll the partition buffer backwards.
         *-----------------------------------------------------------------------*/
        for (int partition_index = 1; partition_index < this->num_partitions; partition_index++)
        {
            memcpy(this->input_history[partition_index - 1],
                   this->input_history[partition_index],
                   sizeof(sample) * this->fft_size);
        }
        memcpy(this->input_history[this->num_partitions - 1],
               this->input->out[hop],
               sizeof(sample) * this->fft_size);
    }
}

}

#endif