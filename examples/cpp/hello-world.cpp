/*------------------------------------------------------------------------
 * Hello, World.
 *
 * The canonical 440hz sine wave example.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

/*------------------------------------------------------------------------
 * All objects are in the signal:: namespace.
 * Import this namespace for code brevity.
 *-----------------------------------------------------------------------*/
using namespace signalflow;

int main()
{
    /*------------------------------------------------------------------------
     * Instantiate a single AudioGraph object for all global audio processing.
     *-----------------------------------------------------------------------*/
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * 440hz Hello World
     *-----------------------------------------------------------------------*/
    NodeRef sine = new SineOscillator(440);

    /*------------------------------------------------------------------------
     * The AudioGraph can have multiple inputs, summed to output.
     *-----------------------------------------------------------------------*/
    graph->play(sine);

    /*------------------------------------------------------------------------
     * Begin audio processing, and run indefinitely.
     *-----------------------------------------------------------------------*/
    graph->start();
    graph->wait();
}
