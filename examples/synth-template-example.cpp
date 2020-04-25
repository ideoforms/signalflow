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
    NodeRef base_freq = tmp->add_input("base_freq", 40.0);
    NodeRef freq = tmp->add_node(new Noise(2.0, true, 40, 160));
    NodeRef sine = tmp->add_node(new Sine(freq + base_freq));
    NodeRef pan_position = tmp->add_input("pan", 0.5);
    NodeRef pan = tmp->add_node(new Pan(2, sine, pan_position));

    /*------------------------------------------------------------------------
     * Set the output of the synth.
     *-----------------------------------------------------------------------*/
    tmp->set_output(pan);

    SynthSpecRef spec = tmp->parse();
    spec->print();

    /*------------------------------------------------------------------------
     * Instantiate two synths that use this template.
     * Pan one hard left, and one hard right.
     *-----------------------------------------------------------------------*/
    SynthRef synth1 = new Synth(tmp);
    synth1->set_input("pan", -1);
    SynthRef synth2 = new Synth(tmp);
    synth2->set_input("pan", 1);

    /*------------------------------------------------------------------------
     * Connect the synths to our graph's output.
     *-----------------------------------------------------------------------*/
    graph->add_output(synth1->output);
    graph->add_output(synth2->output);
    graph->print();
    graph->start();

    while (true)
    {
        float freq = random_uniform(40, 800);
        synth1->set_input("base_freq", freq);
        usleep(250000);
        synth2->set_input("base_freq", freq);
        usleep(250000);
    }

    return 0;
}

