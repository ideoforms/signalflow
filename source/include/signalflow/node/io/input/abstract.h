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

    virtual int init() = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
    virtual int destroy() = 0;

    virtual void process(sample **out, int num_samples) = 0;
};
}
