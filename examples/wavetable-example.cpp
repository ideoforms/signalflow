/*------------------------------------------------------------------------
 * Wavetable example
 *
 * Demonstrates using the `Wavetable` oscillator to generate periodic
 * waveforms from a fixed audio buffer.
 *-----------------------------------------------------------------------*/
#include <signal/signal.h>

using namespace libsignal;

int main()
{
	AudioGraphRef graph = new AudioGraph();

	BufferRef buffer = new EnvelopeBufferHanning(256);
	NodeRef freq = new Noise(2.0, true, 140, 800);
	freq = new RoundToScale(freq);
	NodeRef wavetable = new Wavetable(buffer, freq * 2.0);
	NodeRef delay = new Delay(wavetable, 0.1, 0.3);

	NodeRef pan = new Pan(2, delay);

	NodeRef attenuated = pan * 0.2;
	graph->add_output(attenuated);

	graph->start();
	graph->wait();
}

