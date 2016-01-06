#include "signum/signum.h"

#include <unistd.h>

using namespace signum;

int main()
{
	Graph *graph = new Graph();

	/*------------------------------------------------------------------------
	 * Create a new SynthDef (with optional name)
	 *-----------------------------------------------------------------------*/
	SynthDef *def = new SynthDef();

	/*------------------------------------------------------------------------
	 * Create a named input that can be used to modulate parameters of
	 * the synth.
	 *-----------------------------------------------------------------------*/
	UnitRef base_freq = def->add_input("base_freq", 40.0);
	UnitRef freq = def->add_node(new Noise(2.0, true, 40, 160));
	UnitRef sine = def->add_node(new Sine(freq + base_freq));

	/*------------------------------------------------------------------------
	 * Set the output of the synth.
	 *-----------------------------------------------------------------------*/
	def->set_output(sine);

	/*------------------------------------------------------------------------
	 * Instantiate a synth that uses this definition.
	 *-----------------------------------------------------------------------*/
	Synth *synth1 = new Synth(def);
	graph->output->add_input(synth1->output);

	while (true)
	{
		float freq = rng_uniform(40, 400);
		synth1->set_param("base_freq", freq);
		usleep(500000);
	}


	return 0;
}

