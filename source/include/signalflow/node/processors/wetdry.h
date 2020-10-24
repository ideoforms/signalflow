#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class WetDry : public Node
{
public:
    WetDry(NodeRef dry_input = nullptr,
           NodeRef wet_input = nullptr,
           NodeRef wetness = 0.0);

    virtual void process(sample **out, int num_frames) override;

private:
    NodeRef dry_input;
    NodeRef wet_input;
    NodeRef wetness;
};

REGISTER(WetDry, "wetdry")
}
