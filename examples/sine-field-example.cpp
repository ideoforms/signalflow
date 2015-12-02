#include "signum/signum.h"

#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace signum;

int main()
{
	Graph *graph = new Graph();

	/*------------------------------------------------------------------------
	 * Create a bank of sine bleeps.
	 *-----------------------------------------------------------------------*/
	for (int x = 0; x < 32; x++)
	{
		UnitRef sine = new gen::Sine(440 + rand() % 440);
		UnitRef resample = new fx::Resample(sine, 11025, 12);
		UnitRef noise = new rnd::Noise(0.3, true, 1.0, 2);
		UnitRef dust = new rnd::Dust(noise);
		UnitRef env = new env::ASR(0.002, 0.05, 0.01);
		env->set_param("clock", dust);

		UnitRef sum = resample * env;
		UnitRef delay = new fx::Delay(sum, 0.2, 0.4);
		UnitRef pan = new fx::Pan(2, delay * 0.1, (rand() % 100) / 100.0);
		graph->output->add_input(pan);
	}

	graph->run();
}

