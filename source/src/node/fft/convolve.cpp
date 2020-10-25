#ifdef __APPLE__

#include "signalflow/node/fft/convolve.h"
#include "signalflow/node/fft/fft.h"

namespace signalflow
{

FFTConvolve::FFTConvolve(NodeRef input, BufferRef buffer)
    : FFTOpNode(input), buffer(buffer)
{
    this->name = "fft-convolve";

    if (!buffer)
    {
        throw std::runtime_error("No buffer specified");
    }
    this->num_partitions = ceil((buffer->get_num_frames() - this->fft_size) / this->hop_size) + 1;
    if (this->num_partitions < 1)
        this->num_partitions = 1;
    this->ir_partitions.resize(this->num_partitions);
    this->input_history.resize(this->num_partitions);
    this->output_partition_polar_split = new sample[this->num_bins * 2]();
    this->output_partition_polar = new sample[this->num_bins * 2]();
    this->output_partition_cartesian = new sample[this->num_bins * 2]();
    this->output_sum_cartesian = new sample[this->num_bins * 2]();
    this->output_sum_polar = new sample[this->num_bins * 2]();

    printf("Buffer length %d frames, fft size %d, hop size %d, doing %d partitions\n",
           buffer->get_num_frames(), this->fft_size, this->hop_size, this->num_partitions);

    FFT *fft = new FFT(nullptr, this->fft_size, this->hop_size, this->window_size, false);
    for (int i = 0; i < this->num_partitions; i++)
    {
        this->ir_partitions[i] = new sample[this->num_bins * 2]();
        this->input_history[i] = new sample[this->num_bins * 2]();
        fft->fft(this->buffer->data[0] + i * this->hop_size,
                 this->ir_partitions[i],
                 true,
                 false);
    }
    delete fft;

    this->create_buffer("buffer", this->buffer);
}

FFTConvolve::~FFTConvolve()
{
    for (auto partition : this->ir_partitions)
    {
        delete partition;
    }
}

void FFTConvolve::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Each hop, roll the partition buffer backwards.
         *-----------------------------------------------------------------------*/
        for (int partition_index = this->num_partitions - 2; partition_index >= 0; partition_index--)
        {
            memcpy(this->input_history[partition_index + 1],
                   this->input_history[partition_index],
                   sizeof(sample) * this->num_bins * 2);
        }
        memcpy(this->input_history[0],
               this->input->out[hop],
               sizeof(sample) * this->num_bins * 2);

        memset(output_sum_cartesian, 0, sizeof(sample) * this->num_bins * 2);

        /*------------------------------------------------------------------------
         * Iterate over fft_size frames as each block contains a whole
         * fft of samples.
         *-----------------------------------------------------------------------*/
        for (int partition_index = 0; partition_index < this->num_partitions; partition_index++)
        {
            vDSP_vmul(this->input_history[partition_index], 1,
                      this->ir_partitions[partition_index], 1,
                      this->output_partition_polar_split, 1,
                      this->num_bins);
            vDSP_vadd(this->input_history[partition_index] + this->num_bins, 1,
                      this->ir_partitions[partition_index] + this->num_bins, 1,
                      this->output_partition_polar_split + this->num_bins, 1,
                      this->num_bins);

            const DSPSplitComplex output_partition_polar_split_struct = {
                this->output_partition_polar_split,
                this->output_partition_polar_split + num_bins
            };
            vDSP_ztoc(&output_partition_polar_split_struct, 1,
                      (DSPComplex *) this->output_partition_polar, 2,
                      num_bins);

            vDSP_rect(this->output_partition_polar, 2,
                      this->output_partition_cartesian, 2,
                      this->num_bins);

            vDSP_vadd(this->output_partition_cartesian, 1,
                      this->output_sum_cartesian, 1,
                      this->output_sum_cartesian, 1,
                      this->num_bins * 2);
        } // partitions

        vDSP_polar(this->output_sum_cartesian, 2,
                   this->output_sum_polar, 2,
                   this->num_bins);
        DSPSplitComplex buffer_split = { this->out[hop], this->out[hop] + this->num_bins };
        vDSP_ctoz((DSPComplex *) this->output_sum_polar, 2,
                  &buffer_split, 1,
                  this->num_bins);
        //        this->out[hop][0] = this->input->out[hop][0];
        //        this->out[hop][this->num_bins] = this->input->out[hop][this->num_bins];
    }
}

}

#endif
