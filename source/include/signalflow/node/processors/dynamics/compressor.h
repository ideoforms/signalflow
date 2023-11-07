#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Dynamic range compression, with optional sidechain input.
 *---------------------------------------------------------------------------------*/
class Compressor : public UnaryOpNode
{
public:
    Compressor(NodeRef input = 0.0,
               NodeRef threshold = 0.1,
               NodeRef ratio = 2,
               NodeRef attack_time = 0.01,
               NodeRef release_time = 0.1,
               NodeRef sidechain = nullptr);

    virtual void process(Buffer &out, int num_frames);

    NodeRef threshold;
    NodeRef ratio;
    NodeRef attack_time;
    NodeRef release_time;
    NodeRef sidechain;

private:
    float current_ratio;
};

REGISTER(Compressor, "compressor")
}
