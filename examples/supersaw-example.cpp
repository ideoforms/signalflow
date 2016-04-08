/*------------------------------------------------------------------------
 * Supersaw example:
 *
 * Demonstrates using Signal's multichannel expansion and mixdown
 * to create complex harmonics.
 *-----------------------------------------------------------------------*/

#include "signal/signal.h"

#include <unistd.h>
#include <stdlib.h>

using namespace libsignal;

int main()
{
	/*------------------------------------------------------------------------
	 * Create a global Graph object
	 *-----------------------------------------------------------------------*/
	GraphRef graph = new Graph();

	/*------------------------------------------------------------------------
	 * Create 6 mono channels of sawtooth with varying frequencies.
	 * This is shorthand for:
	 *
	 * NodeRef multiplex = new Multiplex({ 58.3, 59.1, 60.0, 60.3, 60.5 });
	 * NodeRef saw = new Saw(multiplex);
	 *-----------------------------------------------------------------------*/
	NodeRef saw = new Saw({ 58.3, 59.1, 60.0, 60.3, 60.5 });

	/*------------------------------------------------------------------------
	 * Mixer up-mixes or down-mixes between different numbers of channels.
	 * Downmixing 6 channels to 2 creates a stereo spread of six sawtooths
	 * between the L and R speaker.
	 *-----------------------------------------------------------------------*/
	NodeRef mix = new Mixer(saw, 2);

	/*------------------------------------------------------------------------
	 * To add some life, add a resonant filter with wandering cutoff
	 *-----------------------------------------------------------------------*/
	// NodeRef moog = new MoogVCF(mix, (new Sine(0.1))->scale(200, 8000), 2);
	NodeRef moog = new MoogVCF(mix, 200, 2);

	graph->output->add_input(moog);
	graph->run();
}
