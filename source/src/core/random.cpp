/*--------------------------------------------------------------------*
 * util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#include "signalflow/core/random.h"
#include "signalflow/core/util.h"
#include <sys/time.h>

#ifdef HAVE_GSL

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#endif

#include <limits.h>

namespace signalflow
{

/*--------------------------------------------------------------------*
 * Maintain a single global RNG state object.
 *--------------------------------------------------------------------*/
#ifdef HAVE_GSL

gsl_rng *rng;

#endif

/*--------------------------------------------------------------------*
 * random_init(): Initialise pseudo-random number generator.
 *
 * Uses the GNU Scientific Library:
 *   <http://www.gnu.org/software/gsl/>
 *
 *--------------------------------------------------------------------*/
void random_init()
{
#ifdef HAVE_GSL

    struct timeval tv;
    gsl_rng_env_setup();
    rng = gsl_rng_alloc(gsl_rng_default);

    /*--------------------------------------------------------------------*
     * Seed with current time multiplied by microsecond part, to give
     * a pretty decent non-correlated seed.
     *--------------------------------------------------------------------*/
    gettimeofday(&tv, 0);
    random_seed(tv.tv_sec * tv.tv_usec);

#endif
}

void random_seed(long seed)
{
#ifdef HAVE_GSL

    gsl_rng_set(rng, seed);

#endif
}

/*--------------------------------------------------------------------*
 * Generate from gaussian distribution.
 *--------------------------------------------------------------------*/
double random_gaussian(double mean, double sd)
{
    // double value = mean + gsl_ran_gaussian(rng, sd);
    double value = mean + sd * random_gaussian();
    return value;
}

double random_gaussian()
{
#ifdef HAVE_GSL

    double value = gsl_ran_gaussian(rng, 1);

#else

    double u1 = arc4random_uniform(1);
    double u2 = arc4random_uniform(1);
    double f1 = sqrt(-2 * log(u1));
    double f2 = 2 * M_PI * u2;
    double value = f1 * cos(f2);

#endif

    return value;
}

/*--------------------------------------------------------------------*
 * Generate uniform rand.
 *--------------------------------------------------------------------*/
double random_uniform()
{
#ifdef HAVE_GSL

    double value = gsl_rng_uniform(rng);

#else

    double value = (float) arc4random() / UINT_MAX;

#endif

    return value;
}

double random_uniform(double to)
{
    double value = random_uniform() * to;
    return value;
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

float random_exponential(float mu)
{
#ifdef HAVE_GSL
    return gsl_ran_exponential(rng, mu);
#else
    return 0.0;
#endif
}

float random_cauchy(float a)
{
#ifdef HAVE_GSL
    return gsl_ran_cauchy(rng, a);
#else
    return 0.0;
#endif
}

float random_beta(float a, float b)
{
#ifdef HAVE_GSL
    return gsl_ran_beta(rng, a, b);
#else
    return 0.0;
#endif
}

float random_gamma(float a, float b)
{
#ifdef HAVE_GSL
    return gsl_ran_gamma(rng, a, b);
#else
    return 0.0;
#endif
}

float random_levy(float c, float alpha)
{
#ifdef HAVE_GSL
    return gsl_ran_levy(rng, c, alpha);
#else
    return 0.0;
#endif
}

float random_exponential_pdf(float x, float mu)
{
#ifdef HAVE_GSL
    return gsl_ran_exponential_pdf(x, mu);
#else
    return 0.0;
#endif
}

float random_cauchy_pdf(float x, float a)
{
#ifdef HAVE_GSL
    return gsl_ran_cauchy_pdf(x, a);
#else
    return 0.0;
#endif
}

float random_beta_pdf(float x, float a, float b)
{
#ifdef HAVE_GSL
    return gsl_ran_beta_pdf(x, a, b);
#else
    return 0.0;
#endif
}

float random_gamma_pdf(float x, float a, float b)
{
#ifdef HAVE_GSL
    return gsl_ran_gamma_pdf(x, a, b);
#else
    return 0.0;
#endif
}

/*--------------------------------------------------------------------*
 * random_integer: random int, up to (but excluding) to
 *--------------------------------------------------------------------*/
unsigned long random_integer(unsigned long to)
{
    return (long) random_uniform(to);
}

unsigned long random_integer(unsigned long from, unsigned long to)
{
    return from + (((long) random_uniform(to)) % (to - from));
}

} /* namespace signalflow */
