#include "signalflow/buffer/waveshaper-buffer.h"

namespace signalflow
{

WaveShaperBuffer::WaveShaperBuffer(int length)
    : Buffer(1, length)
{
    /*-------------------------------------------------------------------------
     * Initialise to a 1-to-1 linear mapping.
     *-----------------------------------------------------------------------*/
    this->fill([](float input) { return input; });
}

WaveShaperBuffer::WaveShaperBuffer(const std::function<float(float)> f)
    : Buffer(1, 1024)
{
    this->fill(f);
}

double WaveShaperBuffer::offset_to_frame(double offset)
{
    return signalflow_scale_lin_lin(offset, -1, 1, 0, this->num_frames - 1);
}

double WaveShaperBuffer::frame_to_offset(double frame)
{
    return signalflow_scale_lin_lin(frame, 0, this->num_frames - 1, -1, 1);
}

}
