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

    int sample_rate = 0;

    virtual int init() = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
    virtual int destroy() = 0;

    virtual void add_input(NodeRef node);
    virtual void remove_input(NodeRef node);

    std::list<NodeRef> audio_inputs;
    int input_index;
};

} // namespace signalflow
