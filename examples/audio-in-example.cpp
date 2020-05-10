/*------------------------------------------------------------------------
 * Audio input example
 *
 * Takes audio input from the default input device, records it to
 * a short buffer, and plays it back at variable rates.
 * 
 * Make some noise when starting the patch.
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

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
     * Create a one-second stereo buffer.
     *-----------------------------------------------------------------------*/
    BufferRef buffer = new Buffer(2, graph->sample_rate * 1.0);

    /*------------------------------------------------------------------------
     * Create a BufferRecorder to perform a one-shot recording of the input.
     *-----------------------------------------------------------------------*/
    NodeRef recorder = new BufferRecorder(buffer, input);
    graph->add_output(recorder);

    /*------------------------------------------------------------------------
     * Create a parallel series of BufferPlayers to play back the recording.
     * Add a varispeed effect with interpolated Noise objects assigned to
     * the sampler's rate input.
     *-----------------------------------------------------------------------*/
    for (int i = 0; i < 5; i++)
    {
        NodeRef rate = new Noise(0.5, true, 0.3, 1.8);
        NodeRef sampler = new BufferPlayer(buffer, rate, true);

        /*------------------------------------------------------------------------
         * Attenuate the output level so we don't distort.
         *-----------------------------------------------------------------------*/
        graph->add_output(sampler * 0.2);
    }

    graph->start();
    graph->wait();
}
