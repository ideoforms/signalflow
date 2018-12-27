#include "index.h"

namespace libsignal
{

Index::Index(PropertyRef list, NodeRef index) : Node(), list(list), index(index)
{
    this->name = "index";

    this->add_input("index", this->index);
    this->add_property("list", this->list);
}

void Index::process(sample **out, int num_frames)
{
    std::vector <float> list = this->list->float_array_value();

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
