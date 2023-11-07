#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Takes wet and dry inputs, and outputs a mix determined by wetness.
 *---------------------------------------------------------------------------------*/
class WetDry : public Node
{
public:
    WetDry(NodeRef dry_input = nullptr,
           NodeRef wet_input = nullptr,
           NodeRef wetness = 0.0);

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef dry_input;
    NodeRef wet_input;
    NodeRef wetness;
};

REGISTER(WetDry, "wetdry")
}
