#include "signalflow/node/operators/scale.h"

namespace signalflow
{

ScaleLinExp::ScaleLinExp(NodeRef input, NodeRef a, NodeRef b, NodeRef c, NodeRef d)
    : UnaryOpNode(input), a(a), b(b), c(c), d(d)
{
    this->name = "scale-lin-exp";

    this->create_input("a", this->a);
    this->create_input("b", this->b);
    this->create_input("c", this->c);
    this->create_input("d", this->d);
}

void ScaleLinExp::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            float norm = (input->out[channel][frame] - a->out[channel][frame]) / (b->out[channel][frame] - a->out[channel][frame]);
            out[channel][frame] = powf(d->out[channel][frame] / c->out[channel][frame], norm) * c->out[channel][frame];
        }
    }
}

ScaleLinLin::ScaleLinLin(NodeRef input, NodeRef a, NodeRef b, NodeRef c, NodeRef d)
    : UnaryOpNode(input), a(a), b(b), c(c), d(d)
{
    this->name = "scale-lin-lin";

    this->create_input("a", this->a);
    this->create_input("b", this->b);
    this->create_input("c", this->c);
    this->create_input("d", this->d);
}

void ScaleLinLin::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            float norm = (input->out[channel][frame] - a->out[channel][frame]) / (b->out[channel][frame] - a->out[channel][frame]);
            out[channel][frame] = (c->out[channel][frame]) + (d->out[channel][frame] - c->out[channel][frame]) * norm;
        }
    }
}

}
