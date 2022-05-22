/*--------------------------------------------------------------------*
 * util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#include "signalflow/core/random.h"
#include "signalflow/core/util.h"
#include <random>

#include <limits.h>

namespace signalflow
{

/*--------------------------------------------------------------------*
 * Maintain a single global RNG state object.
 *--------------------------------------------------------------------*/
std::mt19937 rng;

/*--------------------------------------------------------------------*
 * random_init(): Initialise pseudo-random number generator.
 *
 *--------------------------------------------------------------------*/
void random_init()
{
    random_seed(signalflow_create_random_seed());
}

void random_seed(long seed)
{
    rng.seed(seed);
}

/*--------------------------------------------------------------------*
 * Generate from gaussian distribution.
 *--------------------------------------------------------------------*/
double random_gaussian(double mean, double sd)
{
    double value = mean + sd * random_gaussian();
    return value;
}

double random_gaussian()
{
    return std::normal_distribution<double>(0, 1)(rng);
}

/*--------------------------------------------------------------------*
 * Generate uniform rand.
 *--------------------------------------------------------------------*/
double random_uniform()
{
    return std::uniform_real_distribution<double>(0, 1)(rng);
}

double random_uniform(double from, double to)
{
    double value = random_uniform();
    value = value * (to - from) + from;
    return value;
}

bool random_coin(double limit = 0.5)
{
    double value = random_uniform();
    return value < limit;
}

float random_exponential(float from, float to)
{
    return signalflow_scale_lin_exp(random_uniform(), 0, 1, from, to);
}

float random_exponential_dist(float mu)
{
    return std::exponential_distribution<double>(mu)(rng);
}

/*--------------------------------------------------------------------*
 * random_integer: random int, up to (but excluding) to
 *--------------------------------------------------------------------*/
unsigned long random_integer(unsigned long to)
{
    return (long) random_uniform(0, to);
}

unsigned long random_integer(unsigned long from, unsigned long to)
{
    return from + (((long) random_uniform(0, to)) % (to - from));
}

} /* namespace signalflow */
