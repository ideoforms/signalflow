/*--------------------------------------------------------------------*
 * util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#include "util.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <limits.h>

namespace signum
{

/*--------------------------------------------------------------------*
 * Maintain a single global RNG state object.
 *--------------------------------------------------------------------*/
gsl_rng *rng;


/*--------------------------------------------------------------------*
 * rng_init(): Initialise pseudo-random number generator.
 *
 * Uses the GNU Scientific Library:
 *   <http://www.gnu.org/software/gsl/>
 *
 *--------------------------------------------------------------------*/
void rng_init()
{
	struct timeval tv;
	
	gsl_rng_env_setup();
	rng = gsl_rng_alloc(gsl_rng_default);
	
	/*--------------------------------------------------------------------*
	 * Seed with current time multiplied by microsecond part, to give
	 * a pretty decent non-correlated seed.
	 *--------------------------------------------------------------------*/
	gettimeofday(&tv, 0);
	rng_seed(tv.tv_sec * tv.tv_usec);
}

void rng_seed(long seed)
{
	gsl_rng_set(rng, seed);
}

/*--------------------------------------------------------------------*
 * Generate from gaussian distribution.
 *--------------------------------------------------------------------*/
double rng_gaussian(double mean, double sd)
{
	double value = mean + gsl_ran_gaussian(rng, sd);
	return value;
}

double rng_gaussian()
{
	double value = gsl_ran_gaussian(rng, 1);
	return value;
}


/*--------------------------------------------------------------------*
 * Generate uniform rand.
 *--------------------------------------------------------------------*/
double rng_uniformuf()
{
	double value = gsl_rng_uniform(rng);
	return value;
}

double rng_uniform(double to)
{
	double value = gsl_rng_uniform(rng) * to;
	return value;
}

double rng_uniform(double from, double to)
{
	double value = gsl_rng_uniform(rng);
	value = value * (to - from) + from;
	return value;
}

bool rng_coin(double limit = 0.5)
{
	double value = gsl_rng_uniform(rng);
	return value < limit;
}


/*--------------------------------------------------------------------*
 * rng_randint: random int, up to (but excluding) to
 *--------------------------------------------------------------------*/
unsigned long rng_randint(unsigned long to)
{
	return gsl_rng_get(rng) % to;
}

unsigned long rng_randint(unsigned long from, unsigned long to)
{
	return from + (gsl_rng_get(rng) % (to - from));
}


/*--------------------------------------------------------------------*
 * timestamp(): Return microsecond-accurate timestamp.
 * Useful for monitoring function runtimes.
 *--------------------------------------------------------------------*/
double timestamp()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (tv.tv_usec / 1000000.0);	
}

/*--------------------------------------------------------------------*
 * clip(): Constrain a value within two bounds.
 *--------------------------------------------------------------------*/
double clip(double value, double min, double max)
{
	if (value < min) value = min;
	if (value > max) value = max;
	return value;
}

/*--------------------------------------------------------------------*
 * map(): Map a value onto a linear range.
 *--------------------------------------------------------------------*/
double map(double value, double fromA, double fromB, double toA, double toB)
{
	double norm = (value - fromA) / (fromB - fromA);
	return toA + norm * (toB - toA);
}

/*--------------------------------------------------------------------*
 * linexp(): Map a value onto an exponential range.
 *--------------------------------------------------------------------*/
double linexp (double value, double a, double b, double c, double d)
{
    if (value <= a) return c;
    if (value >= b) return d;
    return pow(d/c, (value-a)/(b-a)) * c;
}


} /* namespace signum */
