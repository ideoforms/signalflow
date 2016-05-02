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
	 * Load audio buffer
	 *-----------------------------------------------------------------------*/
	Buffer *buffer = new Buffer("audio/gliss.aif");
	NodeRef dust = new Dust(1000.0);
	NodeRef pos = new Noise(0.3, false, 0, buffer->duration);
	NodeRef len = new Noise(100, false, 0.1, 0.5);
	NodeRef pan = new Noise(100, false);
	NodeRef granulator = new Granulator(buffer, dust, pos, len);
	Buffer *env_buf = new EnvelopeBufferHanning();
	granulator->set_input("pan", pan);
	granulator->set_buffer("envelope", env_buf);
	granulator = granulator * 0.25;

	/*------------------------------------------------------------------------
	 * The Graph can have multiple inputs, summed to output.
	 *-----------------------------------------------------------------------*/
	graph->add_output(granulator);

	/*------------------------------------------------------------------------
	 * Begin audio processing, and loop forever.
	 *-----------------------------------------------------------------------*/
	graph->start();
	graph->wait();
}

