#include <signal/signal.h>

using namespace libsignal;

int main()
{
	Graph *graph = new Graph();

	/*------------------------------------------------------------------------
	 * Create a bank of sine bleeps.
	 *-----------------------------------------------------------------------*/
	for (int x = 0; x < 32; x++)
	{
		UnitRef sine = new Sine(rng_uniform(220, 1660));
		UnitRef resample = new Resample(sine, 11025, 12);
		UnitRef noise = new Noise(0.3, true, 1.0, 2);
		UnitRef dust = new Dust(noise);
		UnitRef env = new ASR(0.005, 0.01, 0.05);
		env->set_param("clock", dust);

		UnitRef sum = resample * env;
		UnitRef pan = new Pan(2, sum * 0.1, rng_uniform(0, 1));
		UnitRef delay = new Delay(pan, 0.2, 0.4);
		graph->output->add_input(delay);
	}

	graph->run();
}

