/*------------------------------------------------------------------------
 * Supersaw example:
 *
 * Demonstrates using Signal's multichannel expansion and mixdown
 * to create complex harmonics.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

using namespace signalflow;

int main()
{
    /*------------------------------------------------------------------------
     * Create a global AudioGraph object
     *-----------------------------------------------------------------------*/
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Create 6 mono channels of sawtooth with varying frequencies.
     * This is shorthand for:
     *
     * NodeRef multiplex = new ChannelArray({ 58.3, 59.1, 60.0, 60.3, 60.5 });
     * NodeRef saw = new Saw(multiplex);
     *-----------------------------------------------------------------------*/
    NodeRef saw = new Saw({ 58.3, 59.1, 60.0, 60.3, 60.5 });

    /*------------------------------------------------------------------------
     * ChannelMixer up-mixes or down-mixes between different numbers of channels.
     * Downmixing 6 channels to 2 creates a stereo spread of six sawtooths
     * between the L and R speaker.
     *-----------------------------------------------------------------------*/
    NodeRef mix = new ChannelMixer(2, saw);

    /*------------------------------------------------------------------------
     * To add some life, add a resonant filter with wandering cutoff
     *-----------------------------------------------------------------------*/
    NodeRef moog = new MoogVCF(mix, (new Sine(0.1))->scale(200, 8000), 2);

    graph->play(moog);
    graph->start();
    graph->wait();
}
