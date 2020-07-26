#pragma once

#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

class AudioOut_Abstract : public Node
{
public:
    AudioOut_Abstract();
    virtual void process(sample **out, int num_samples);

    virtual int init() = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
    virtual int destroy() = 0;

    virtual void add_input(NodeRef node);
    virtual void remove_input(NodeRef node);

    unsigned int get_sample_rate();

    /**--------------------------------------------------------------------------------
     * Returns the buffer size required by the audio hardware.
     *
     * @return The buffer size, in frames.
     *-------------------------------------------------------------------------------*/
    unsigned int get_buffer_size();

protected:
    unsigned int sample_rate = 0;
    unsigned int buffer_size = 0;
    std::list<NodeRef> audio_inputs;
    int input_index;
};

} // namespace signalflow
