/*------------------------------------------------------------------------
 * JSON load example
 *
 * Demonstrates loading a synth structure from a JSON graph description.
 * Optional pathname to a JSON file can be passed in argv.
 *-----------------------------------------------------------------------*/

#include "signal/signal.h"

using namespace libsignal;

int main(int argc, char **argv)
{
    GraphRef graph = new Graph();

	/*------------------------------------------------------------------------
	 * A Structure contains a description of a synthesis graph.
	 * It takes a short human-readable name, drawn from [0-9a-zA-Z_-]
	 *-----------------------------------------------------------------------*/
    StructRef structure = new Structure("synth");

	/*------------------------------------------------------------------------
	 * ->load() parses the specified JSON file and creates a graph 
	 * definition from it. 
	 *-----------------------------------------------------------------------*/
	if (argc > 1)
		structure->load(argv[1]);
	else
		structure->load("synths/sine-env-delay.json");

	/*------------------------------------------------------------------------
	 * Instantiate a synth using this structure.
	 * TODO: Instantiate two synths with different frequencies/pans.
	 *-----------------------------------------------------------------------*/
    SynthRef synth = new Synth(structure);
    graph->output->add_input(synth->output);
    graph->run();

    return 0;
}
