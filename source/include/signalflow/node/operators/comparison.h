#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Equal : public BinaryOpNode
{
public:
    Equal(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

class NotEqual : public BinaryOpNode
{
public:
    NotEqual(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

class GreaterThan : public BinaryOpNode
{
public:
    GreaterThan(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

class GreaterThanOrEqual : public BinaryOpNode
{
public:
    GreaterThanOrEqual(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

class LessThan : public BinaryOpNode
{
public:
    LessThan(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

class LessThanOrEqual : public BinaryOpNode
{
public:
    LessThanOrEqual(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

class Modulo : public BinaryOpNode
{
public:
    Modulo(NodeRef a = 0, NodeRef b = 0);
    virtual void process(Buffer &out, int num_frames);
};

class Abs : public UnaryOpNode
{
public:
    Abs(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames);
};

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
