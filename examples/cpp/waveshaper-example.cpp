/*------------------------------------------------------------------------
 * Waveshaper example
 *
 * Demonstrates constructing a waveshaper buffer from a lambda function,
 * used to dynamically alter the timbre of an oscillator.
 *-----------------------------------------------------------------------*/
#include <signalflow/signalflow.h>

using namespace signalflow;

int main()
{
    /*------------------------------------------------------------------------
     * Create the global signal processing graph.
     *-----------------------------------------------------------------------*/
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Create a stereo pair of oscillating sine waves, an octave apart.
     *-----------------------------------------------------------------------*/
    NodeRef sine = new SineOscillator({ 40, 80 });

    /*------------------------------------------------------------------------
     * Create a WaveShaperBuffer and populate it with a cubic function.
     * WaveShaperBuffer is a subclass of Buffer that is optimised for
     * mapping a sample value [-1, 1] to a new sample value [-1, 1].
     *-----------------------------------------------------------------------*/
    BufferRef buffer = new WaveShaperBuffer();
    buffer->fill([](float input) { return input * input * input; });

    /*------------------------------------------------------------------------
     * The WaveShaper node takes an input audio node and a WaveShaperBuffer.
     * Distort the input by multiplying it to exceed [-1, 1].
     * WaveShaper will clip this to [-1, 1] (as well as mapping the samples
     * based on the buffer's cubic function).
     *-----------------------------------------------------------------------*/
    NodeRef overdriven = sine * new WhiteNoise(0.2, 1, 3, true);
    NodeRef shaper = new WaveShaper(overdriven, buffer);

    /*------------------------------------------------------------------------
     * Add some delay and stereo width modulation.
     *-----------------------------------------------------------------------*/
    NodeRef delay = new CombDelay(shaper, 0.5, 0.5, 0.5);
    NodeRef width = new SineOscillator(0.2);
    NodeRef throb = new StereoWidth(delay, width.scale(0.5, 1));

    /*------------------------------------------------------------------------
     * Send to output and start processing.
     *-----------------------------------------------------------------------*/
    graph->play(throb);
    graph->wait();
}
