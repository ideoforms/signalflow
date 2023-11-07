#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Scales the input from a linear range (between a and b)
 * to an exponential range (between c and d).
 *---------------------------------------------------------------------------------*/
class ScaleLinExp : public UnaryOpNode
{

public:
    ScaleLinExp(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10);

    virtual void process(Buffer &out, int num_frames);

    NodeRef a, b, c, d;
};

/**--------------------------------------------------------------------------------*
 * Scales the input from a linear range (between a and b)
 * to a linear range (between c and d).
 *---------------------------------------------------------------------------------*/
class ScaleLinLin : public UnaryOpNode
{

public:
    ScaleLinLin(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10);

    virtual void process(Buffer &out, int num_frames);

    NodeRef a, b, c, d;
};

REGISTER(ScaleLinExp, "scale-lin-exp")
REGISTER(ScaleLinLin, "scale-lin-lin")
}
