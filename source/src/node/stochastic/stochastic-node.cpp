#include "signalflow/node/stochastic/stochastic-node.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

namespace signalflow
{

StochasticNode::StochasticNode(NodeRef reset)
    : reset(reset)
{
    this->create_input("reset", this->reset);

    struct timeval tv;
    this->rng = gsl_rng_alloc(gsl_rng_default);

    /*--------------------------------------------------------------------*
     * Seed with current time multiplied by microsecond part, to give
     * a pretty decent non-correlated seed.
     *--------------------------------------------------------------------*/
    gettimeofday(&tv, 0);
    this->set_seed(tv.tv_sec * tv.tv_usec);
}

double StochasticNode::random_uniform(double from, double to)
{
    double value = gsl_rng_uniform(this->rng);
    value = value * (to - from) + from;
    return value;
}

void StochasticNode::set_seed(unsigned long int seed)
{
    this->seed = seed;
    gsl_rng_set(this->rng, this->seed);
}

void StochasticNode::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_TRIGGER_RESET)
    {
        gsl_rng_set(this->rng, this->seed);
    }
    else
    {
        throw std::runtime_error("Unknown trigger: " + name);
    }
}

}
