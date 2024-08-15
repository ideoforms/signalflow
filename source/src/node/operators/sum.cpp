#include "signalflow/core/core.h"
#include "signalflow/node/operators/sum.h"
#include "signalflow/node/oscillators/constant.h"

namespace signalflow
{

void Sum::init()
{
    this->name = "sum";
}

Sum::Sum()
    : VariableInputNode()
{
    this->init();
}

Sum::Sum(std::initializer_list<NodeRef> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

Sum::Sum(std::vector<NodeRef> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

Sum::Sum(std::vector<float> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

Sum::Sum(std::vector<int> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

void Sum::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        memset(this->out[channel], 0, sizeof(sample) * num_frames);
        for (NodeRef input : this->input_list)
        {
#ifdef __APPLE__
            vDSP_vadd(input->out[channel], 1, out[channel], 1, out[channel], 1, num_frames);
#else
            for (int frame = 0; frame < num_frames; frame++)
            {
                out[channel][frame] += input->out[channel][frame];
            }
#endif
        }
    }
}

}
