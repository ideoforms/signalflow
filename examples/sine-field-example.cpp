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
		NodeRef sine = new Sine(random_uniform(220, 1660));
		NodeRef resample = new Resample(sine, 11025, 12);
		NodeRef noise = new Noise(0.3, true, 1.0, 2);
		NodeRef dust = new Dust(noise);
		NodeRef env = new ASR(0.005, 0.01, 0.05);
		env->set_input("clock", dust);

		NodeRef sum = resample * env;
		NodeRef pan = new Pan(2, sum * 0.1, random_uniform(0, 1));
		NodeRef delay = new Delay(pan, 0.2, 0.4);
		graph->add_output(delay);
	}

	graph->start();
	graph->wait();
}

