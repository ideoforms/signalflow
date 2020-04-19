#include "signal/node/operators/scale.h"

namespace libsignal
{

LinExp::LinExp(NodeRef input, NodeRef a, NodeRef b, NodeRef c, NodeRef d)
    : UnaryOpNode(input), a(a), b(b), c(c), d(d)
{
    this->name = "linexp";

    this->add_input("a", this->a);
    this->add_input("b", this->b);
    this->add_input("c", this->c);
    this->add_input("d", this->d);
}

void LinExp::process(sample **out, int num_frames)
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

Scale::Scale(NodeRef input, NodeRef a, NodeRef b, NodeRef c, NodeRef d)
    : UnaryOpNode(input), a(a), b(b), c(c), d(d)
{
    this->name = "scale";

    this->add_input("a", this->a);
    this->add_input("b", this->b);
    this->add_input("c", this->c);
    this->add_input("d", this->d);
}

void Scale::process(sample **out, int num_frames)
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
