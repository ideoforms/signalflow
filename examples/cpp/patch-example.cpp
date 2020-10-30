/*------------------------------------------------------------------------
 * Patch template example
 *
 * Demonstrates the creation of a reusable graph of nodes, that can
 * be subsequently replicated for polyphonic output.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

#include <unistd.h>

using namespace signalflow;

PatchSpecRef create_synth()
{
    /*------------------------------------------------------------------------
     * Create a new PatchTemplate
     *-----------------------------------------------------------------------*/
    PatchRef patch = new Patch();

    /*------------------------------------------------------------------------
     * Create a named input that can be used to modulate parameters of
     * the patch.
     *-----------------------------------------------------------------------*/
    NodeRef freq = patch->add_input("freq", 440.0);
    NodeRef width = patch->add_input("width", 0.5);
    NodeRef pan = patch->add_input("pan", 0.0);
    NodeRef square = patch->add_node(new Square(freq, width));
    NodeRef asr = patch->add_node(new EnvelopeASR(0.0, 0.0, 1.3));
    NodeRef shaped = patch->add_node(square * asr * 0.05);
    NodeRef stereo = patch->add_node(new LinearPanner(2, shaped, pan));
    NodeRef delay = new CombDelay(stereo, 0.1, 0.9, 0.5);
    NodeRef output = stereo + delay * 0.3;
    patch->set_output(output);

    PatchSpecRef ref = patch->create_spec();
    return ref;
}

int main()
{
    AudioGraphRef graph = new AudioGraph();

    PatchSpecRef spec = create_synth();
    spec->print();

    graph->start();
    graph->show_status(2);

    std::set<PatchRef> synths;

    while (true)
    {
        float freq = 100 * int(powf(8, random_uniform(0, 1)));
        PatchRef patch = new Patch(spec, { { "freq", freq }, { "width", random_uniform(0.3, 0.7) } });
        synths.insert(patch);
        patch->set_auto_free(true);
        graph->play(patch);
        usleep(1000000);
    }

    return 0;
}
