#include "signal/filters/freeze.h"

namespace libsignal
{

Freeze::Freeze(NodeRef input) : UnaryOpNode(input)
{
    this->buffer = new Buffer(1, 2048);

    this->name = "freeze";

    this->frozen = false;
    this->trig = false;
}

void Freeze::process(sample **out, int num_frames)
{
    if (this->trig || !this->frozen)
    {
        memcpy(this->buffer->data[0], this->input->out[0], sizeof(sample) * num_frames);
        this->trig = false;
    }

    memcpy(out[0], this->buffer->data[0], sizeof(sample) * num_frames);
}

void Freeze::trigger(std::string name, float value)
{
    this->frozen = true;
    this->trig = true;
}

};
