#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Outputs sin(a), per sample.
 *---------------------------------------------------------------------------------*/
class Sin : public UnaryOpNode
{
public:
    Sin(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames) override;
};

/**--------------------------------------------------------------------------------*
 * Outputs cos(a), per sample.
 *---------------------------------------------------------------------------------*/
class Cos : public UnaryOpNode
{
public:
    Cos(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames) override;
};

/**--------------------------------------------------------------------------------*
 * Outputs tan(a), per sample.
 *---------------------------------------------------------------------------------*/
class Tan : public UnaryOpNode
{
public:
    Tan(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames) override;
};

/**--------------------------------------------------------------------------------*
 * Outputs tanh(a), per sample.
 * Can be used as a soft clipper.
 *---------------------------------------------------------------------------------*/
class Tanh : public UnaryOpNode
{
public:
    Tanh(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(Sin, "sin")
REGISTER(Cos, "cos")
REGISTER(Tan, "tan")
REGISTER(Tanh, "tanh")

}
