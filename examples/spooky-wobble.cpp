/*------------------------------------------------------------------------
 * Tape-wobble effect with crackle.
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

using namespace libsignal;

int main()
{
	AudioGraphRef graph = new AudioGraph();

	/*------------------------------------------------------------------------
	 * Load in a sample and play it back at a varying rate.
	 *-----------------------------------------------------------------------*/
	BufferRef buffer = new Buffer("audio/gliss.aif");
	NodeRef sampler = new Sampler(buffer, new Noise(0.3, true), true);
	NodeRef sampler_pan = new Pan(2, sampler, 0.25);
	graph->add_output(sampler_pan);

	/*------------------------------------------------------------------------
	 * Add some gentle crackle.
	 *-----------------------------------------------------------------------*/
	NodeRef noise = new Noise(50, true);
	NodeRef noise_pan = new Pan(2, noise, 0.75);
	graph->add_output(noise_pan);

	/*------------------------------------------------------------------------
	 * Add some occasional pops.
	 *-----------------------------------------------------------------------*/
	NodeRef dust = new Dust(1);
	NodeRef dust_pan = new Pan(2, dust, 0.5);
	graph->add_output(dust_pan);

	/*------------------------------------------------------------------------
	 * Begin playback.
	 *-----------------------------------------------------------------------*/
	graph->start();
	graph->wait();
}
