#pragma once

#include "signalflow/node/node.h"

#include <random>
//#include <sys/time.h>

#define SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()             \
    if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->reset, channel, frame)) \
    {                                                                  \
        this->StochasticNode::trigger(SIGNALFLOW_TRIGGER_RESET);       \
    }

namespace signalflow
{

class StochasticNode : public Node
{
public:
    StochasticNode(NodeRef reset = nullptr);
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void set_seed(unsigned long int seed);

protected:
    double random_uniform(double from = 0.0, double to = 1.0);
    double random_gaussian(double mean = 0.0, double sigma = 1.0);
    double random_exponential(double lambda = 1.0);

    NodeRef reset;
    unsigned long int seed;

    std::mt19937 rng;
};

}
