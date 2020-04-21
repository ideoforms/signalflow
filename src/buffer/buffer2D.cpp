#include "signal/buffer/buffer2D.h"

#include <vector>

namespace libsignal
{

Buffer2D::Buffer2D(BufferRef bufferA, BufferRef bufferB)
    : bufferA(bufferA), bufferB(bufferB)
{
    if (bufferA->num_channels != 1 || bufferB->num_channels != 1)
    {
        throw std::runtime_error("Input buffers to Buffer2D must both be mono");
    }
    if (bufferA->num_frames != bufferB->num_frames)
    {
        throw std::runtime_error("Input buffers to Buffer2D must both have identical length");
    }
    if (bufferA->sample_rate != bufferB->sample_rate)
    {
        throw std::runtime_error("Input buffers to Buffer2D must both have identical sample rate");
    }

    this->num_channels = 1;
    this->num_frames = bufferA->num_frames;
    this->sample_rate = bufferA->sample_rate;
    this->duration = this->num_frames / this->sample_rate;
}

sample Buffer2D::get2D(double offset_x, double offset_z)
{
    sample a = this->bufferA->get(offset_x);
    sample b = this->bufferB->get(offset_x);
    return ((1.0 - offset_z) * a) + (offset_z * b);
}

}
