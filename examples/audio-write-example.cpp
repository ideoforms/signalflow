/*------------------------------------------------------------------------
 * Audio write example
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

#include <unistd.h>
#include <stdlib.h>

using namespace libsignal;

int main()
{
	/*------------------------------------------------------------------------
	 * Instantiate a global processing graph.
	 *-----------------------------------------------------------------------*/
	AudioGraphRef graph = new AudioGraph();

	/*------------------------------------------------------------------------
	 * Take a two-channel input from the default input device.
	 *-----------------------------------------------------------------------*/
	NodeRef input = new AudioIn();

	/*------------------------------------------------------------------------
	 * Create a five-second stereo buffer.
	 *-----------------------------------------------------------------------*/
	Buffer *buffer = new Buffer(2, graph->sample_rate * 1.0);

	/*------------------------------------------------------------------------
	 * Create a Recorder to perform a one-shot recording of the input.
	 *-----------------------------------------------------------------------*/
	NodeRef recorder = new Recorder(buffer, input);
	graph->add_output(recorder);
	graph->start();

	/*------------------------------------------------------------------------
	 * Wait until our recording is complete (happening in the audio I/O
	 * thread), then save to disk.
	 *-----------------------------------------------------------------------*/
	usleep(1e6);
	buffer->save("out.wav");
}
