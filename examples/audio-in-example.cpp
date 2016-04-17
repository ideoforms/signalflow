/*------------------------------------------------------------------------
 * Audio input example
 *
 * Takes audio input from the default input device, records it to
 * a short buffer, and plays it back at variable rates.
 * 
 * Make some noise when starting the patch.
 *-----------------------------------------------------------------------*/

#include "signal/signal.h"

#include <unistd.h>
#include <stdlib.h>

using namespace libsignal;

int main()
{
	/*------------------------------------------------------------------------
	 * Instantiate a global processing graph.
	 *-----------------------------------------------------------------------*/
	GraphRef graph = new Graph();

	/*------------------------------------------------------------------------
	 * Take a two-channel input from the default input device.
	 *-----------------------------------------------------------------------*/
	NodeRef input = new AudioIn();

	/*------------------------------------------------------------------------
	 * Create a one-second stereo buffer.
	 *-----------------------------------------------------------------------*/
	Buffer *buffer = new Buffer(2, graph->sample_rate * 1.0);

	/*------------------------------------------------------------------------
	 * Create a Recorder to perform a one-shot recording of the input.
	 *-----------------------------------------------------------------------*/
	NodeRef recorder = new Recorder(buffer, input);
	graph->output->add_input(recorder);

	/*------------------------------------------------------------------------
	 * Create a parallel series of Samplers to play back the recording.
	 * Add a varispeed effect with interpolated Noise objects assigned to
	 * the sampler's rate input.
	 *-----------------------------------------------------------------------*/
	for (int i = 0; i < 5; i++)
	{
		NodeRef rate = new Noise(0.5, true, 0.3, 1.8);
		NodeRef sampler = new Sampler(buffer, rate, true);

		/*------------------------------------------------------------------------
		 * Attenuate the output level so we don't distort.
		 *-----------------------------------------------------------------------*/
		graph->output->add_input(sampler * 0.2);
	}

	graph->run();
}
