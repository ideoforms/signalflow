#include "signalflow/node/operators/trigonometry.h"
#include <math.h>

namespace signalflow
{

Sin::Sin(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "sin";
}

void Sin::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = sinf(this->input->out[channel][frame]);
        }
    }
}

Cos::Cos(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "cos";
}

void Cos::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = cosf(this->input->out[channel][frame]);
        }
    }
}

Tan::Tan(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "tan";
}

void Tan::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = tanf(this->input->out[channel][frame]);
        }
    }
}

Tanh::Tanh(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "tanh";
}

void Tanh::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = tanhf(this->input->out[channel][frame]);
        }
    }
}

}
