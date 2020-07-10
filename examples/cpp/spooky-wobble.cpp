/*------------------------------------------------------------------------
 * Spooky Wobble
 *
 * Tape-wobble effect with crackle.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

using namespace signalflow;

int main()
{
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Load in a sample and play it back at a varying rate.
     *-----------------------------------------------------------------------*/
    BufferRef buffer = new Buffer("audio/gliss.aif");
    NodeRef sampler = new BufferPlayer(buffer, new Noise(0.3, true), true);
    NodeRef sampler_pan = new LinearPanner(2, sampler, 0.25);
    graph->add_output(sampler_pan);

    /*------------------------------------------------------------------------
     * Add some gentle crackle.
     *-----------------------------------------------------------------------*/
    NodeRef noise = new Noise(50, true);
    NodeRef noise_pan = new LinearPanner(2, noise, 0.75);
    graph->add_output(noise_pan);

    /*------------------------------------------------------------------------
     * Add some occasional pops.
     *-----------------------------------------------------------------------*/
    NodeRef dust = new RandomImpulse(1);
    NodeRef dust_pan = new LinearPanner(2, dust, 0.5);
    graph->add_output(dust_pan);

    /*------------------------------------------------------------------------
     * Begin playback.
     *-----------------------------------------------------------------------*/
    graph->start();
    graph->wait();
}
