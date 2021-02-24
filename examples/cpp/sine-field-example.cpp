/*------------------------------------------------------------------------
 * SineOscillator field example
 *
 * An array of delayed sine pings.
 *-----------------------------------------------------------------------*/

#include <signalflow/signalflow.h>

using namespace signalflow;

int main()
{
    /*------------------------------------------------------------------------
     * Create the global processing graph and begin playback.
     *-----------------------------------------------------------------------*/
    AudioGraphRef graph = new AudioGraph();
    graph->start();

    /*------------------------------------------------------------------------
     * Create a bank of sine bleeps.
     *-----------------------------------------------------------------------*/
    for (int x = 0; x < 32; x++)
    {
        NodeRef sine = new SineOscillator(random_uniform(220, 1660));
        NodeRef resample = new Resample(sine, 11025, 12);
        NodeRef noise = new WhiteNoise(0.3, true, 1.0, 2);
        NodeRef dust = new RandomImpulse(noise);
        NodeRef env = new EnvelopeASR(0.005, 0.01, 0.05);
        env->set_input("clock", dust);

        NodeRef sum = resample * env;
        NodeRef pan = new LinearPanner(2, sum * 0.1, random_uniform(-1, 1));
        NodeRef delay = new CombDelay(pan, 0.2, 0.4);
        graph->play(delay);
    }

    /*------------------------------------------------------------------------
     * Run forever.
     *-----------------------------------------------------------------------*/
    graph->wait();
}
