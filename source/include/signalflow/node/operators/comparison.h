#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Compares the output of a to the output of b. Outputs 1 when equal, 0 otherwise.
 * Can also be written as a == b
 *---------------------------------------------------------------------------------*/
class Equal : public BinaryOpNode
{
public:
    Equal(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Compares the output of a to the output of b. Outputs 0 when equal, 1 otherwise.
 * Can also be written as a != b
 *---------------------------------------------------------------------------------*/
class NotEqual : public BinaryOpNode
{
public:
    NotEqual(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Compares the output of a to the output of b. Outputs 1 when a > b, 0 otherwise.
 * Can also be written as a > b
 *---------------------------------------------------------------------------------*/
class GreaterThan : public BinaryOpNode
{
public:
    GreaterThan(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Compares the output of a to the output of b. Outputs 1 when a >= b, 0 otherwise.
 * Can also be written as a >= b
 *---------------------------------------------------------------------------------*/
class GreaterThanOrEqual : public BinaryOpNode
{
public:
    GreaterThanOrEqual(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Compares the output of a to the output of b. Outputs 1 when a < b, 0 otherwise.
 * Can also be written as a < b
 *---------------------------------------------------------------------------------*/
class LessThan : public BinaryOpNode
{
public:
    LessThan(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Compares the output of a to the output of b. Outputs 1 when a <= b, 0 otherwise.
 * Can also be written as a <= b
 *---------------------------------------------------------------------------------*/
class LessThanOrEqual : public BinaryOpNode
{
public:
    LessThanOrEqual(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Outputs the value of a modulo b, per sample. Supports fractional values.
 * Can also be written as a % b
 *---------------------------------------------------------------------------------*/
class Modulo : public BinaryOpNode
{
public:
    Modulo(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Outputs the absolute value of a, per sample.
 * Can also be written as abs(a)
 *---------------------------------------------------------------------------------*/
class Abs : public UnaryOpNode
{
public:
    Abs(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames);
};

/**--------------------------------------------------------------------------------*
 * Outputs value_if_true for each non-zero value of a, value_if_false for all
 * other values.
 *---------------------------------------------------------------------------------*/
class If : public UnaryOpNode
{
public:
    If(NodeRef a = 0, NodeRef value_if_true = 0, NodeRef value_if_false = 0);
    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef value_if_true;
    NodeRef value_if_false;
};

REGISTER(Equal, "equals")
REGISTER(NotEqual, "not-equal")
REGISTER(GreaterThan, "greater-than")
REGISTER(GreaterThanOrEqual, "greater-than-or-equal")
REGISTER(LessThan, "less-than")
REGISTER(LessThanOrEqual, "less-than-or-equal")
REGISTER(Modulo, "modulo")
REGISTER(Abs, "abs")
REGISTER(If, "if")

}
