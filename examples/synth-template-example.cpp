/*------------------------------------------------------------------------
 * Synth template example
 *
 * Demonstrates the creation of a reusable graph of nodes, that can
 * be subsequently replicated for polyphonic output.
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

#include <unistd.h>

using namespace libsignal;

SynthSpecRef create_synth()
{
    /*------------------------------------------------------------------------
     * Create a new SynthTemplate 
     *-----------------------------------------------------------------------*/
    SynthRef synth = new Synth();

    /*------------------------------------------------------------------------
     * Create a named input that can be used to modulate parameters of
     * the synth.
     *-----------------------------------------------------------------------*/
    NodeRef freq   = synth->add_input("freq", 440.0);
    NodeRef width  = synth->add_input("width", 0.5);
    NodeRef pan    = synth->add_input("pan", 0.5);
    NodeRef square = synth->add_node(new Square(freq, width));
    NodeRef asr    = synth->add_node(new EnvelopeASR(0.0, 0.0, 1.3));
    NodeRef shaped = synth->add_node(square * asr * 0.05);
    NodeRef stereo = synth->add_node(new LinearPanner(2, shaped, pan));
    NodeRef delay = new Delay(stereo, 0.1, 0.9, 0.5);
    // NodeRef output = stereo + delay * 0.3;
    NodeRef output = stereo * 0.2;
    synth->set_output(output);

    SynthSpecRef ref = synth->parse();
    return ref;
}

int main()
{
    AudioGraphRef graph = new AudioGraph();

    SynthSpecRef spec = create_synth();
    spec->print();

    graph->start();
    graph->poll(2);

    std::set<SynthRef> synths;

    while (true)
    {
        float freq = 110 * int(powf(8, random_uniform(0, 1)));
        SynthRef synth = new Synth(spec);
        synths.insert(synth);
        synth->set_auto_free(true);
        synth->set_input("freq", freq);
        synth->set_input("width", random_uniform(0.3, 0.7));
        graph->add_output(synth);
        usleep(1000000);
    }

    return 0;
}

