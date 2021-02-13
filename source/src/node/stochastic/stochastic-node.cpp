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
    seed = tv.tv_sec * tv.tv_usec;
    gsl_rng_set(rng, seed);
}

void StochasticNode::trigger(std::string name, float value)
{
    gsl_rng_set(rng, seed);
}

}
