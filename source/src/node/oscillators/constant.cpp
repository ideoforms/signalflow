#include "signalflow/node/oscillators/constant.h"

namespace signalflow
{

Constant::Constant(sample value)
    : Node()
{
    this->value = value;
    this->name = "constant";
    this->set_channels(0, 1);

    /*--------------------------------------------------------------------------------
     * Unlike most other nodes, Constant renders output immediately.
     * This is done so that it can be used to assign to a Node's parameter and
     * its output queried immediately. This can be particularly useful in live
     * coding contexts. eg:
     *
     * node.probability = 0.5    # creates a Constant(0.5)
     * node.trigger("generate")  # examines the Constant's ->out property
     *--------------------------------------------------------------------------------*/
    this->process(this->out, this->output_buffer_length);
}

void Constant::process(Buffer &out, int num_frames)
{
#if __APPLE__
    vDSP_vfill(&(this->value), out[0], 1, num_frames);
#else
    std::fill(out[0], out[0] + num_frames, this->value);
#endif
}

float Constant::get_value()
{
    return this->value;
}

void Constant::set_value(float value)
{
    this->value = value;
}

}
