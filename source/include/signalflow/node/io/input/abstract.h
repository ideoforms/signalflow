#pragma once

#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{

class AudioIn_Abstract : public Node
{
public:
    AudioIn_Abstract();

    virtual void init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void destroy() = 0;

    virtual void process(Buffer &out, int num_samples) = 0;
};

}
