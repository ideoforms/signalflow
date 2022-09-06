/*------------------------------------------------------------------------
 * Spooky Wobble
 *
 * Tape-wobble effect with crackle.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

using namespace signalflow;

int main()
{
#ifdef HAVE_SNDFILE
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Load in a sample and play it back at a varying rate.
     *-----------------------------------------------------------------------*/
    BufferRef buffer = new Buffer("audio/gliss.aif");
    NodeRef sampler = new BufferPlayer(buffer, new WhiteNoise(0.3, true), true);
    NodeRef sampler_pan = new StereoPanner(sampler, -0.5);
    graph->play(sampler_pan);

    /*------------------------------------------------------------------------
     * Add some gentle crackle.
     *-----------------------------------------------------------------------*/
    NodeRef noise = new WhiteNoise(50, true);
    NodeRef noise_pan = new StereoPanner(noise, 0.5);
    graph->play(noise_pan);

    /*------------------------------------------------------------------------
     * Add some occasional pops.
     *-----------------------------------------------------------------------*/
    NodeRef dust = new RandomImpulse(1);
    NodeRef dust_pan = new SteoreoPanner(dust, 0.0);
    graph->play(dust_pan);

    /*------------------------------------------------------------------------
     * Loop indefinitely.
     *-----------------------------------------------------------------------*/
    graph->wait();

#else
    std::cerr << "signalflow was not built with libsndfile" << std::endl;
    return 0;
#endif
}
