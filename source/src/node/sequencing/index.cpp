#include "signalflow/node/sequencing/index.h"

namespace signalflow
{

Index::Index(PropertyRef list, NodeRef index)
    : Node(), list(list), index(index)
{
    this->name = "index";

    this->create_input("index", this->index);
    this->create_property("list", this->list);
}

void Index::process(Buffer &out, int num_frames)
{
    std::vector<float> list = this->list->float_array_value();

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            int index = (int) this->index->out[channel][frame];
            out[channel][frame] = list[index];
        }
    }
}

}
