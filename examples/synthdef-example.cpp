#include <signal/signal.h>

#include <unistd.h>

using namespace libsignal;

int main()
{
	Graph *graph = new Graph();

	/*------------------------------------------------------------------------
	 * Create a new Structure 
	 *-----------------------------------------------------------------------*/
	StructRef structure = new Structure("my_synth");

	/*------------------------------------------------------------------------
	 * Create a named input that can be used to modulate parameters of
	 * the synth.
	 *-----------------------------------------------------------------------*/
	NodeRef base_freq = structure->add_input("base_freq", 40.0);
	NodeRef freq = structure->add_node(new Noise(2.0, true, 40, 160));
	NodeRef sine = structure->add_node(new Sine(freq + base_freq));
	NodeRef pan_position = structure->add_input("pan", 0.5);
	NodeRef pan = structure->add_node(new Pan(2, sine, pan_position));

	/*------------------------------------------------------------------------
	 * Set the output of the synth.
	 *-----------------------------------------------------------------------*/
	structure->set_output(pan);

	/*------------------------------------------------------------------------
	 * Instantiate two synths that use this structure.
	 * Pan one hard left, and one hard right.
	 *-----------------------------------------------------------------------*/
	SynthRef synth1 = new Synth(structure);
	synth1->set_input("pan", 0);
	SynthRef synth2 = new Synth(structure);
	synth2->set_input("pan", 1);

	/*------------------------------------------------------------------------
	 * Connect the synths to our graph's output.
	 *-----------------------------------------------------------------------*/
	graph->add_output(synth1->output);
	graph->add_output(synth2->output);

	while (true)
	{
		float freq = random_uniform(40, 800);
		synth1->set_input("base_freq", freq);
		usleep(250000);
		synth2->set_input("base_freq", freq);
		usleep(250000);
	}


	return 0;
}

