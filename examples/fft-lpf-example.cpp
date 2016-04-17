/*------------------------------------------------------------------------
 * FFT brick-wall filter example.
 *
 * Performs an FFT on an incoming signal, then passes it through
 * a frequency-domain brick wall filter, zeroing any bins beyond
 * the specified cutoff frequency.
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
	 * Load and play a sample.
	 *-----------------------------------------------------------------------*/
	Buffer *buffer = new Buffer("audio/gliss.aif");
	NodeRef sampler = new Sampler(buffer, 1.0, true);

	/*------------------------------------------------------------------------
	 * Perform FFT -> filter -> inverse FFT
	 *-----------------------------------------------------------------------*/
	NodeRef fft = new FFT(sampler);
	NodeRef lpf = new FFTLPF(fft, 400);
	NodeRef output = new IFFT(lpf);

	/*------------------------------------------------------------------------
	 * Pan the output to centre of stereo field.
	 *-----------------------------------------------------------------------*/
	NodeRef pan = new Pan(2, output);

	/*------------------------------------------------------------------------
	 * Send to output.
	 *-----------------------------------------------------------------------*/
	graph->output->add_input(pan);
	graph->run();
}
