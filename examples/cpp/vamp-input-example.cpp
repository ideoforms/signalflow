/*------------------------------------------------------------------------
 * Vamp example
 *
 * Uses the Vamp audio analysis framework to extract features from
 * the audio input.
 *
 * SignalFlow must be compiled with Vamp support.
 *
 * Docs:
 * https://www.vamp-plugins.org/plugin-doc/vamp-example-plugins.html
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

using namespace signalflow;

int main()
{
    AudioGraphRef graph = new AudioGraph();
    NodeRef input = new AudioIn();
    NodeRef centroid = new VampAnalysis(input, "vamp:vamp-example-plugins:spectralcentroid:linearcentroid");

    centroid->poll(10, "centroid");

    NodeRef sine = new SineOscillator(centroid);

    graph->play(sine);
    graph->wait();
}
