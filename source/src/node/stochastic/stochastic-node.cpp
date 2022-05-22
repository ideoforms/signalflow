#include "signalflow/node/stochastic/stochastic-node.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

namespace signalflow
{

StochasticNode::StochasticNode(NodeRef reset)
    : Node(), reset(reset)
{
    this->create_input("reset", this->reset);
    this->set_seed(signalflow_create_random_seed());
}

double StochasticNode::random_uniform(double from, double to)
{
    return std::uniform_real_distribution<double>(from, to)(this->rng);
}

double StochasticNode::random_gaussian(double mean, double sigma)
{
    return std::normal_distribution<double>(mean, sigma)(this->rng);
}

double StochasticNode::random_exponential_dist(double lambda)
{
    return std::exponential_distribution<double>(lambda)(this->rng);
}

double StochasticNode::random_exponential(double from, double to)
{
    return signalflow_scale_lin_exp(this->random_uniform(0, 1), 0, 1, from, to);
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
