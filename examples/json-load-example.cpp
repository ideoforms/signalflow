/*------------------------------------------------------------------------
 * JSON load example
 *
 * Demonstrates loading a synth spec from a JSON graph description.
 * Optional pathname to a JSON file can be passed in argv.
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

using namespace libsignal;

int main(int argc, char **argv)
{
	GraphRef graph = new Graph();

	/*------------------------------------------------------------------------
	 * A SynthSpec contains a description of a synthesis graph.
	 * It takes a short human-readable name, drawn from [0-9a-zA-Z_-]
	 *-----------------------------------------------------------------------*/
	SynthSpecRef spec = new SynthSpec("synth");

	/*------------------------------------------------------------------------
	 * ->load() parses the specified JSON file and creates a graph 
	 * definition from it. 
	 *-----------------------------------------------------------------------*/
	if (argc > 1)
		spec->load(argv[1]);
	else
		spec->load("synths/sine-env-delay.json");

	/*------------------------------------------------------------------------
	 * Instantiate a synth using this spec.
	 * TODO: Instantiate two synths with different frequencies/pans.
	 *-----------------------------------------------------------------------*/
	SynthRef synth = new Synth(spec);
	graph->add_output(synth->output);
	graph->start();
	graph->wait();

	return 0;
}
