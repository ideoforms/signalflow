#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/sine.h"

namespace signalflow
{

SineOscillator::SineOscillator(NodeRef frequency, NodeRef phase_offset, NodeRef reset)
    : frequency(frequency), phase_offset(phase_offset), reset(reset)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "sine";
    this->create_input("frequency", this->frequency);
    this->create_input("phase_offset", this->phase_offset);
    this->create_input("reset", this->reset);

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

void SineOscillator::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

void SineOscillator::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * Precalculate phase increment for efficiency
     *--------------------------------------------------------------------------------*/
    float phase_increment_scale = M_PI * 2.0 / this->graph->get_sample_rate();
    Node *frequency = this->frequency.get();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_TRIGGER(this->reset, frame))
            {
                this->phase[channel] = 0;
            }

            out[channel][frame] = this->phase[channel];

            this->phase[channel] += frequency->out[channel][frame] * phase_increment_scale;

            /*--------------------------------------------------------------------------------
             * This formulation is much more efficient than fmod().
             * Performing a single modulo after the for(frame) loop was trialled, but
             * resulted in rounding errors and aliasing in the corresponding output frequency
             * even when using a double for phase - not sure why.
             *--------------------------------------------------------------------------------*/
            while (this->phase[channel] > M_PI * 2)
            {
                this->phase[channel] -= M_PI * 2;
            }
        }

#ifdef __APPLE__
        if (this->phase_offset)
        {
            vDSP_vadd(out[channel], 1, this->phase_offset->out[channel], 1, out[channel], 1, num_frames);
        }

        vvsinf(out[channel], out[channel], &num_frames);
#else
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (this->phase_offset)
            {
                out[channel][frame] += this->phase_offset->out[channel][frame];
            }
            out[channel][frame] = sinf(out[channel][frame]);
        }
#endif
    }
}

}
