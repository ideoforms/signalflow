/*------------------------------------------------------------------------
 * Waveshaper example
 *
 * Demonstrates constructing a waveshaper buffer from a lambda function,
 * used to dynamically alter the timbre of an oscillator.
 *-----------------------------------------------------------------------*/
#include "signal/signal.h"

using namespace libsignal;

int main()
{
	/*------------------------------------------------------------------------
	 * Create the global signal processing graph.
	 *-----------------------------------------------------------------------*/
	GraphRef graph = new Graph();

	/*------------------------------------------------------------------------
	 * Create a stereo pair of oscillating sine waves, an octave apart.
	 *-----------------------------------------------------------------------*/
	NodeRef sine = new Sine({ 40, 80 });

	/*------------------------------------------------------------------------
	 * Create a WaveShaperBuffer and populate it with a cubic function.
	 * WaveShaperBuffer is a subclass of Buffer that is optimised for
	 * mapping a sample value [-1, 1] to a new sample value [-1, 1].
	 *-----------------------------------------------------------------------*/
	Buffer *buffer = new WaveShaperBuffer();
	buffer->fill([](float input) { return input * input * input; });

	/*------------------------------------------------------------------------
	 * The WaveShaper node takes an input audio node and a WaveShaperBuffer.
	 * Distort the input by multiplying it to exceed [-1, 1].
	 * WaveShaper will clip this to [-1, 1] (as well as mapping the samples
	 * based on the buffer's cubic function).
	 *-----------------------------------------------------------------------*/
	NodeRef overdriven = sine * new Noise(0.2, true, 1, 3);
	NodeRef shaper = new WaveShaper(overdriven, buffer);

	/*------------------------------------------------------------------------
	 * Add some delay and stereo width modulation.
	 *-----------------------------------------------------------------------*/
	NodeRef delay = new Delay(shaper, 0.5, 0.5, 0.5);
	NodeRef width = new Sine(0.2);
	NodeRef throb = new Width(delay, width->scale(0.5, 1));

	/*------------------------------------------------------------------------
	 * Send to output and start processing.
	 *-----------------------------------------------------------------------*/
	graph->output->add_input(throb);
	graph->run();
}
