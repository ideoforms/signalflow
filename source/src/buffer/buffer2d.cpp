#include "signalflow/buffer/buffer2d.h"

#include <vector>

namespace signalflow
{

Buffer2D::Buffer2D(std::vector<BufferRef> buffers)
{
    this->num_channels = buffers[0]->get_num_channels();
    this->num_frames = buffers[0]->get_num_frames();
    this->sample_rate = buffers[0]->get_sample_rate();

    for (auto buffer : buffers)
    {
        if (buffer->get_num_channels() != 1)
        {
            throw std::runtime_error("Input buffers to Buffer2D must all be mono");
        }
        if (buffer->get_num_frames() != this->num_frames)
        {
            throw std::runtime_error("Input buffers to Buffer2D must all have identical length");
        }
        if (buffer->get_sample_rate() != this->sample_rate)
        {
            throw std::runtime_error("Input buffers to Buffer2D must all have identical sample rate");
        }
    }

    this->num_buffers = buffers.size();
    this->duration = this->num_frames / this->sample_rate;
    this->interpolate = SIGNALFLOW_INTERPOLATION_LINEAR;

    /*------------------------------------------------------------------------
     * Data is always a square matrix.
     *------------------------------------------------------------------------*/
    this->data = new sample *[this->num_buffers];
    sample *sample_data = new sample[this->num_buffers * this->num_frames];
    for (int i = 0; i < this->num_buffers; i++)
    {
        this->data[i] = sample_data + i * this->num_frames;

        memcpy(this->data[i], buffers[i]->data[0], this->num_frames * sizeof(sample));
    }
}

Buffer2D::~Buffer2D()
{
    if (this->data)
    {
        // delete this->data[0];
        // delete this->data;
    }
}

sample Buffer2D::get2D(double offset_x, double offset_z)
{
    offset_z *= (this->num_buffers - 1);
    if (this->interpolate == SIGNALFLOW_INTERPOLATION_LINEAR)
    {
        int offset_x_int = int(offset_x);
        int offset_z_int = int(offset_z);
        double offset_x_frac = offset_x - offset_x_int;
        double offset_z_frac = offset_z - offset_z_int;

        sample s00 = this->data[offset_z_int][offset_x_int];
        sample s01 = this->data[offset_z_int][(offset_x_int + 1) % this->num_frames];
        sample s0 = ((1.0 - offset_x_frac) * s00) + (offset_x_frac * s01);
        sample s10 = this->data[(offset_z_int + 1) % this->num_buffers][offset_x_int];
        sample s11 = this->data[(offset_z_int + 1) % this->num_buffers][(offset_x_int + 1) % this->num_frames];
        sample s1 = ((1.0 - offset_x_frac) * s10) + (offset_x_frac * s11);

        sample rv = ((1.0 - offset_z_frac) * s0) + (offset_z_frac * s1);

        return rv;
    }
    else
    {
        int offset_x_int = int(round(offset_x)) % this->num_frames;
        int offset_z_int = int(round(offset_z)) % this->num_buffers;

        return this->data[offset_z_int][offset_x_int];
    }
}

}
