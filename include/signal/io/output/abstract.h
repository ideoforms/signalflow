#pragma once

#include "signal/node.h"
#include "signal/graph.h"
#include "signal/graph.h"

#include <list>


namespace libsignal
{
    
    class AudioOut_Abstract : public Node
    {
    public:
        AudioOut_Abstract(AudioGraph *graph);
        virtual void process(sample **out, int num_samples);

        int sample_rate = 0;

        virtual int init() = 0;
        virtual int start() = 0;
        virtual int stop() = 0;
        virtual int destroy() = 0;

        using Node::add_input;
        using Node::remove_input;
        virtual void add_input(NodeRef node);
        virtual void remove_input(NodeRef node);

        std::list <NodeRef> inputs;
    };

} // namespace libsignal
