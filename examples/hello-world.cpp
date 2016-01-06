#include <signum/signum.h>

/*------------------------------------------------------------------------
 * All objects are in the signum:: namespace.
 * Import this namespace for code brevity.
 *-----------------------------------------------------------------------*/
using namespace signum;

int main()
{
	/*------------------------------------------------------------------------
	 * Instantiate a single Graph object for all global audio processing.
	 *-----------------------------------------------------------------------*/
	Graph *graph = new Graph();

	/*------------------------------------------------------------------------
	 * 440hz Hello World
	 *-----------------------------------------------------------------------*/
	UnitRef sine = new Sine(440);

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

