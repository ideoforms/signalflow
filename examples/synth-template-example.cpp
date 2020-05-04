/*------------------------------------------------------------------------
 * Synth template example
 *
 * Demonstrates the creation of a reusable graph of nodes, that can
 * be subsequently replicated for polyphonic output.
 *-----------------------------------------------------------------------*/

#include <signal/signal.h>

#include <unistd.h>

using namespace libsignal;

int main()
{
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Create a new SynthTemplate 
     *-----------------------------------------------------------------------*/
    SynthTemplateRef tmp = new SynthTemplate("my_synth");

    /*------------------------------------------------------------------------
     * Create a named input that can be used to modulate parameters of
     * the synth.
     *-----------------------------------------------------------------------*/
    NodeRef freq = tmp->add_input("freq", 40.0);
    NodeRef width = tmp->add_input("width", 0.5);
    NodeRef pan = tmp->add_input("pan", 0.5);
    NodeRef square = new Square(freq, width);
    NodeRef asr = new ASR(0.0, 0.0, 1.3);
    NodeRef shaped = square * asr * 0.05;
    NodeRef stereo = new Pan(2, shaped, pan);
    // NodeRef delay = new Delay(stereo, 0.1, 0.9, 0.5);
    // NodeRef output = stereo + delay * 0.3;
    NodeRef output = stereo * 0.2;
    tmp->set_output(output);

    SynthSpecRef spec = tmp->parse();
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

