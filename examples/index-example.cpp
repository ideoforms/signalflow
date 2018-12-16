/*------------------------------------------------------------------------
 * Index example
 *
 * Demonstrates using the `Index` node to retrieve indexed items
 * from a `std::vector`. Note that the first argument to Index is a
 * static property, not a node.
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

#include <unistd.h>
#include <stdlib.h>

using namespace libsignal;

int main()
{
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Create a list of frequencies to iterate over
     *-----------------------------------------------------------------------*/
    std::vector <float> freqs = { 60, 120, 180, 240, 300, 360, 420, 480 };

    /*------------------------------------------------------------------------
     * Create a pair of sawtooth waves, scaled between 0..8, used to index
     * over the array. The Saw nodes are output over consecutive channels.
     *
     * Index casts the `index` inputs to an integer, used to select the
     * output frequency. Via automatic channel upmixing, this creates
     * two parallel counters, phasing gradually against each other.
     *-----------------------------------------------------------------------*/
    NodeRef saw = (new Saw({ 1, 0.97 }))->scale(0, 8);
    NodeRef index = new Index(freqs, saw);

    /*------------------------------------------------------------------------
     * Use the resultant values as frequency parameters of a Sine node,
     * with a time-synced pair of Triangle waves to act as envelopes.
     *-----------------------------------------------------------------------*/
    NodeRef sine = new Sine(index);
    NodeRef envelope = (new Triangle({ 8, 7.76  }))->scale(0, 1);
    sine = sine * envelope;

    /*------------------------------------------------------------------------
     * Reduce the stereo width slightly to increase the interaction between
     * L / R channels.
     *-----------------------------------------------------------------------*/
    NodeRef width = new Width(sine, 0.5);

    graph->add_output(width);
    graph->start();
    graph->wait();
}
