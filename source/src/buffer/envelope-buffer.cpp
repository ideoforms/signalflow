#include "signalflow/buffer/envelope-buffer.h"

namespace signalflow
{

EnvelopeBuffer::EnvelopeBuffer(int length)
    : Buffer(1, length)
{
    /*-------------------------------------------------------------------------
     * Initialise to a flat envelope at maximum amplitude.
     *-----------------------------------------------------------------------*/
    this->fill(1.0);
}

EnvelopeBuffer::EnvelopeBuffer(std::string shape, int num_frames)
    : EnvelopeBuffer(num_frames)
{
    if (shape == "triangle")
    {
        for (int x = 0; x < num_frames / 2; x++)
            this->data[0][x] = (float) x / (num_frames / 2);
        for (int x = 0; x < num_frames / 2; x++)
            this->data[0][(num_frames / 2) + x] = 1.0 - (float) x / (num_frames / 2);
    }
    else if (shape == "linear-decay")
    {
        for (int x = 0; x < num_frames; x++)
            this->data[0][x] = 1.0 - (float) x / num_frames;
    }
    else if (shape == "hanning")
    {
        for (int x = 0; x < num_frames; x++)
        {
            this->data[0][x] = 0.5 * (1.0 - cos(2 * M_PI * x / (num_frames - 1)));
        }
    }
    else if (shape == "rectangular")
    {
        for (int x = 0; x < num_frames; x++)
        {
            this->data[0][x] = 1.0;
        }
    }
    else
    {
        throw std::runtime_error("Invalid buffer shape: " + shape);
    }
}

EnvelopeBuffer::EnvelopeBuffer(const std::function<float(float)> f)
    : Buffer(1, 1024)
{
    this->fill(f);
}

EnvelopeBuffer::EnvelopeBuffer(std::vector<float> samples)
    : Buffer(samples) {}

double EnvelopeBuffer::offset_to_frame(double offset)
{
    return signalflow_scale_lin_lin(offset, 0, 1, 0, this->num_frames - 1);
}

double EnvelopeBuffer::frame_to_offset(double frame)
{
    return signalflow_scale_lin_lin(frame, 0, this->num_frames - 1, 0, 1);
}

}
