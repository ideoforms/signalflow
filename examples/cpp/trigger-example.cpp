/*------------------------------------------------------------------------
 * Trigger example
 *
 * Node triggers are discrete events that trigger a given behaviour
 * within a node. This example demonstrates using a trigger to
 * periodically reset the position of an envelope node.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

#include <stdlib.h>
#include <unistd.h>

using namespace signalflow;

int main()
{
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Create a simple envelope-modulated triangle wave.
     *-----------------------------------------------------------------------*/
    NodeRef triangle = new TriangleOscillator(1000);
    NodeRef envelope = new EnvelopeASR(0.01, 0.0, 0.1);
    NodeRef output = triangle * envelope;

    /*------------------------------------------------------------------------
     * Pan the output across the stereo field.
     *-----------------------------------------------------------------------*/
    NodeRef panned = new StereoPanner(output);

    graph->play(panned);

    while (true)
    {
        /*------------------------------------------------------------------------
         * Periodically, retrigger the envelope, panned to a random location.
         *-----------------------------------------------------------------------*/
        usleep(random_integer(1e4, 1e6));
        panned->set_input("pan", random_uniform(-1, 1));
        envelope->trigger();
    }
}
