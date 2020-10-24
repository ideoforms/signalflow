#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/sine.h"

namespace signalflow
{

Sine::Sine(NodeRef frequency)
    : frequency(frequency)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "sine";
    this->create_input("frequency", this->frequency);

    /*--------------------------------------------------------------------------------
     * This can't be done in Node::Node because base class constructors
     * cannot call subclass virtual functions (because the subclass hasn't yet)
     * been created).
     *
     * For this reason, Node subclasses always need to call their own alloc() from
     * their ctor.
     *
     * https://isocpp.org/wiki/faq/strange-inheritance#calling-virtuals-from-ctors
     *
     * If subsequent reallocation needs to happen, this method will be called via
     * allocate_output_buffer (after calling Node->free() first.)
     *-------------------------------------------------------------------------------*/
    this->alloc();
}

void Sine::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

void Sine::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float freq = this->frequency->out[channel][frame];
            out[channel][frame] = sin(this->phase[channel] * M_PI * 2.0);
            this->phase[channel] += freq / this->graph->get_sample_rate();

            while (this->phase[channel] > 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
