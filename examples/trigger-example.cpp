/*------------------------------------------------------------------------
 * Trigger example
 *
 * Node triggers are discrete events that trigger a given behaviour
 * within a node. This example demonstrates using a trigger to
 * periodically reset the position of an envelope node.
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

#include <unistd.h>
#include <stdlib.h>

using namespace libsignal;

int main()
{
	GraphRef graph = new Graph();

	/*------------------------------------------------------------------------
	 * Create a simple envelope-modulated triangle wave.
	 *-----------------------------------------------------------------------*/
	NodeRef triangle = new Triangle(1000);
	NodeRef envelope = new ASR(0.01, 0.0, 0.1);
	NodeRef output = triangle * envelope;

	/*------------------------------------------------------------------------
	 * Pan the output across the stereo field.
	 *-----------------------------------------------------------------------*/
	NodeRef panned = new Pan(2, output);

	graph->add_output(panned);

	while (true)
	{
		/*------------------------------------------------------------------------
		 * Periodically, retrigger the envelope, panned to a random location.
		 *-----------------------------------------------------------------------*/
		usleep(random_integer(1e4, 1e6));
		panned->set_param("pan", random_uniform(-1, 1));
		envelope->trigger();
	}
}
