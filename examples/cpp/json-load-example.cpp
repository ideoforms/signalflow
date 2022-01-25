/*------------------------------------------------------------------------
 * JSON load example
 *
 * Demonstrates loading a patch spec from a JSON graph description.
 * Optional pathname to a JSON file can be passed in argv.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

using namespace signalflow;

int main(int argc, char **argv)
{
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * A PatchSpec contains a description of a synthesis graph.
     * It takes a short human-readable name, drawn from [0-9a-zA-Z_-]
     *-----------------------------------------------------------------------*/
    PatchSpecRef spec = new PatchSpec();

    /*------------------------------------------------------------------------
     * ->load() parses the specified JSON file and creates a graph 
     * definition from it. 
     *-----------------------------------------------------------------------*/
    if (argc > 1)
        spec->load(argv[1]);
    else
        spec->load("synths/sine-env-delay.json");

    /*------------------------------------------------------------------------
     * Instantiate a patch using this spec.
     * TODO: Instantiate two synths with different frequencies/pans.
     *-----------------------------------------------------------------------*/
    PatchRef patch = new Patch(spec);
    graph->play(patch);
    graph->wait();

    return 0;
}
