/*------------------------------------------------------------------------
 * Mouse control example
 *
 * Demonstrates mapping the mouse cursor position to synthesis
 * parameters.
 *-----------------------------------------------------------------------*/
#include <signal/signal.h>

/*------------------------------------------------------------------------
 * All objects are in the signal:: namespace.
 * Import this namespace for code brevity.
 *-----------------------------------------------------------------------*/
using namespace libsignal;

int main()
{
	/*------------------------------------------------------------------------
	 * Instantiate a global processing graph.
	 *-----------------------------------------------------------------------*/
	AudioGraphRef graph = new AudioGraph();

	/*------------------------------------------------------------------------
	 * Read an audio sample.
	 *-----------------------------------------------------------------------*/
	BufferRef buffer = new Buffer("audio/jurassic-park-mono.wav");

	/*------------------------------------------------------------------------
	 * The MouseX and MouseY objects give live mouse position data,
	 * normalised to [0, 1].
	 *-----------------------------------------------------------------------*/
	NodeRef mousex = new MouseX();
	NodeRef mousey = new MouseY();
	mousex->poll(2);
	mousey->poll(2);

	/*------------------------------------------------------------------------
	 * Use the MouseX to determine the position within a sample for
	 * granulation, plus noise to prevent the machine-gun effect.
	 *-----------------------------------------------------------------------*/
	NodeRef noise = new Noise();
	NodeRef pos = (mousex * buffer->duration) + noise->scale(-1.0, 1.0);

	/*------------------------------------------------------------------------
	 * Create Granulator object, with 50 grains per second.
	 * MouseY determines the playback rate.
	 *-----------------------------------------------------------------------*/
	NodeRef granulator = new Granulator(buffer, new Tick(50), pos, 0.2, mousey * 2.0);
	granulator->set_input("max_grains", 50);
	graph->add_output(granulator * 0.2);
	graph->start();
	graph->wait();
}
