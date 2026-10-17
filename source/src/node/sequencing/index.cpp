#include "signalflow/node/sequencing/index.h"

namespace signalflow
{

Index::Index(std::vector<float> list, NodeRef index)
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
            /*--------------------------------------------------------------------------------
             * Clamp index to the bounds of the list. An empty list outputs zero.
             *--------------------------------------------------------------------------------*/
            if (list.empty())
            {
                out[channel][frame] = 0.0;
                continue;
            }
            int index = (int) this->index->out[channel][frame];
            if (index < 0)
                index = 0;
            else if (index >= (int) list.size())
                index = list.size() - 1;
            out[channel][frame] = list[index];
        }
    }
}

}
