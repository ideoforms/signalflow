#include <signal/signal.h>

/*------------------------------------------------------------------------
 * All objects are in the signal:: namespace.
 * Import this namespace for code brevity.
 *-----------------------------------------------------------------------*/
using namespace libsignal;

int main()
{
	/*------------------------------------------------------------------------
	 * Instantiate a single Graph object for all global audio processing.
	 *-----------------------------------------------------------------------*/
	Graph *graph = new Graph();

	/*------------------------------------------------------------------------
	 * 440hz Hello World
	 *-----------------------------------------------------------------------*/
	NodeRef sine = new Sine(440);

	/*------------------------------------------------------------------------
	 * The Graph can have multiple inputs, summed to output.
	 *-----------------------------------------------------------------------*/
	graph->output->add_input(sine);

	/*------------------------------------------------------------------------
	 * Graph actually begins processing as soon as inputs are added.
	 * run() really just loops indefinitely.
	 *-----------------------------------------------------------------------*/
	graph->run();
}

