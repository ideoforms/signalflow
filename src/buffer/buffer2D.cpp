#include "signal/buffer/buffer2D.h"

#include <vector>

namespace libsignal
{

Buffer2D::Buffer2D(BufferRef bufferA, BufferRef bufferB)
    : bufferA(bufferA), bufferB(bufferB)
{
    assert(bufferA->num_channels == 1);
    assert(bufferB->num_channels == 1);
    assert(bufferA->num_frames == bufferB->num_frames);
    assert(bufferA->sample_rate == bufferB->sample_rate);

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
