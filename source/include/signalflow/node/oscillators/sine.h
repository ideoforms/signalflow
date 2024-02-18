#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Produces a sine wave at the given `frequency`.
 *---------------------------------------------------------------------------------*/
class SineOscillator : public Node
{
public:
    SineOscillator(NodeRef frequency = 440, NodeRef phase_offset = nullptr, NodeRef reset = nullptr);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void alloc() override;

    NodeRef frequency;
    NodeRef phase_offset;
    NodeRef reset;

private:
    std::vector<float> phase;
};

REGISTER(SineOscillator, "sine")
}
