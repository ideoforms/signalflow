#pragma once 

#include "signal/node/node.h"

namespace libsignal
{
    class Line : public Node
    {
    public:
        Line(NodeRef time = 1.0, NodeRef from = 0.0, NodeRef to = 1.0);

        virtual void process(sample **out, int num_frames);

        NodeRef time;
        NodeRef from;
        NodeRef to;

    private:
        float value;
        float value_change_per_step;
        int step;
        int step_target;

    };

    REGISTER(Line, "line");
}
