#include "signalflow/node/stochastic/stochastic-node.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

namespace signalflow
{

StochasticNode::StochasticNode(NodeRef reset)
    : Node(), reset(reset)
{
    this->create_input("reset", this->reset);

    //struct timeval tv;

    /*--------------------------------------------------------------------*
     * Seed with current time multiplied by microsecond part, to give
     * a pretty decent non-correlated seed.
     *--------------------------------------------------------------------*/
    //gettimeofday(&tv, 0);
    //this->rng.seed(tv.tv_sec * tv.tv_usec);
    auto t = signalflow_seconds_since_midnight();
    this->rng.seed(int(t) + int(t * 1e6));
}

double StochasticNode::random_uniform(double from, double to)
{
    return std::uniform_real_distribution<double>(from, to)(this->rng);
}

double StochasticNode::random_gaussian(double mean, double sigma)
{
    return std::normal_distribution<double>(mean, sigma)(this->rng);
}

double StochasticNode::random_exponential(double lambda)
{
    return std::exponential_distribution<double>(lambda)(this->rng);
}

void StochasticNode::set_seed(unsigned long int seed)
{
    this->seed = seed;
    this->rng.seed(seed);
}

void StochasticNode::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_TRIGGER_RESET)
    {
        this->rng.seed(this->seed);
    }
    else
    {
        throw std::runtime_error("Unknown trigger: " + name);
    }
}

}
