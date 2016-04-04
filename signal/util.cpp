/*--------------------------------------------------------------------*
 * util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#include "util.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#ifdef HAVE_GSL

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#endif

#include <limits.h>

namespace libsignal
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

/*--------------------------------------------------------------------*
 * explin(): Map a value from an exponential to linear range.
 *--------------------------------------------------------------------*/
double explin (double value, double a, double b, double c, double d)
{
    if (value <= a) return c;
    if (value >= b) return d;
	return (log(value / a)) / (log(b / a)) * (d - c) + c;
}


float freq_to_midi(float frequency)
{
	/*--------------------------------------------------------------------*
	 * freq_to_midi(): Map a frequency value onto a MIDI note index.
	 * 440 = A4 = 69
	 *--------------------------------------------------------------------*/
	return 69.0 + 12 * log2(frequency / 440.0);
}


float midi_to_freq(float midi)
{
	/*--------------------------------------------------------------------*
	 * midi_to_freq(): Map a MIDI note to a frequency value.
	 * 440 = A4 = 69
	 *--------------------------------------------------------------------*/
	return 440.0 * powf(2, (midi - 69) / 12.0);
}

} /* namespace libsignal */
