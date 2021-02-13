#pragma once

#include "signalflow/node/node.h"

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <sys/time.h>

namespace signalflow
{

class StochasticNode : public Node
{
public:
    StochasticNode(NodeRef reset = nullptr);
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

protected:
    NodeRef reset;
    unsigned long int seed;
    gsl_rng *rng;
};

}
