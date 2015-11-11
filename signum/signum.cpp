#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <sys/time.h>

#include "util.h"

using namespace signum;

void signum_init()
{
	rng_init();
}

int signum_samplerate()
{
	return 44100.0;
}
