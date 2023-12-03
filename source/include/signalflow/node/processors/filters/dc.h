#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Remove low-frequency and DC content from a signal.
 *---------------------------------------------------------------------------------*/
class DCFilter : public UnaryOpNode
{
public:
    DCFilter(NodeRef input = 0.0);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void alloc() override;

private:
    float R;
    std::vector<float> previous_input;
    std::vector<float> previous_output;
};

REGISTER(DCFilter, "dc-filter")
}
