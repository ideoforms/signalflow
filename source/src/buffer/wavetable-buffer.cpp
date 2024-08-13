#include "signalflow/buffer/wavetable-buffer.h"

namespace signalflow
{

WavetableBuffer::WavetableBuffer(int length)
    : Buffer(1, length)
{
    /*-------------------------------------------------------------------------
     * Initialise to a flat envelope at maximum amplitude.
     *-----------------------------------------------------------------------*/
    this->fill(1.0);
}

WavetableBuffer::WavetableBuffer(std::string shape, int num_frames)
    : WavetableBuffer(num_frames)
{
    if (shape == "sine")
    {
        for (int x = 0; x < num_frames; x++)
        {
            this->data[0][x] = sinf(2 * M_PI * x / num_frames);
        }
    }
    else if (shape == "triangle")
    {
        for (int x = 0; x < num_frames / 2; x++)
            this->data[0][x] = (float) 4 * x / num_frames - 1;
        for (int x = 0; x < num_frames / 2; x++)
            this->data[0][(num_frames / 2) + x] = 1 - 4 * (float) x / (num_frames);
    }
    else if (shape == "saw")
    {
        for (int x = 0; x < num_frames; x++)
            this->data[0][x] = 2.0 * x / num_frames - 1.0;
    }
    else if (shape == "square")
    {
        for (int x = 0; x < num_frames; x++)
        {
            if (x < num_frames / 2)
                this->data[0][x] = -1.0;
            else
                this->data[0][x] = 1.0;
        }
    }
    else
    {
        throw std::runtime_error("Invalid wavetable buffer shape: " + shape);
    }
}

WavetableBuffer::WavetableBuffer(const std::function<float(float)> f)
    : Buffer(1, 1024)
{
    this->fill(f);
}

WavetableBuffer::WavetableBuffer(std::vector<float> samples)
    : Buffer(samples) {}

double WavetableBuffer::offset_to_frame(double offset)
{
    return signalflow_scale_lin_lin(offset, 0, 1, 0, this->num_frames - 1);
}

double WavetableBuffer::frame_to_offset(double frame)
{
    return signalflow_scale_lin_lin(frame, 0, this->num_frames - 1, 0, 1);
}

}
