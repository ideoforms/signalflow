/*------------------------------------------------------------------------
 * Granulator example
 *
 * Demonstrates granular synthesis upon an audio buffer, with randomly
 * modulated position and length, and a user-specified grain envelope.
 *-----------------------------------------------------------------------*/
#include <signal/signal.h>

/*------------------------------------------------------------------------
 * All objects are in the signal:: namespace.
 * Import this namespace for code brevity.
 *-----------------------------------------------------------------------*/
using namespace libsignal;

int main()
{
    /*------------------------------------------------------------------------
     * Instantiate a single AudioGraph object for all global audio processing.
     *-----------------------------------------------------------------------*/
    AudioGraphRef graph = new AudioGraph();

    /*------------------------------------------------------------------------
     * Load audio buffer
     *-----------------------------------------------------------------------*/
    BufferRef buffer = new Buffer("audio/gliss.aif");

    /*------------------------------------------------------------------------
     * Dust creates an impulse train with randomised interval, at a 
     * given mean frequency. This is used to trigger grains.
     *-----------------------------------------------------------------------*/
    NodeRef dust = new Dust(100.0);

    /*------------------------------------------------------------------------
     * Set position, length and pan to oscillating randomised values.
     *-----------------------------------------------------------------------*/
    NodeRef pos = new Noise(0.3, false, 0, buffer->duration);
    NodeRef len = new Noise(100, false, 0.1, 0.5);
    NodeRef pan = new Noise(100, false);

    /*------------------------------------------------------------------------
     * Create a granulator object with Hanning envelope applies to grains.
     *-----------------------------------------------------------------------*/
    NodeRef granulator = new Granulator(buffer, dust, pos, len);
    BufferRef env_buf = new EnvelopeBufferHanning();
    granulator->set_input("pan", pan);
    granulator->set_buffer("envelope", env_buf);
    granulator = granulator * 0.25;

    /*------------------------------------------------------------------------
     * The AudioGraph can have multiple inputs, summed to output.
     * Add the granulator as its only output.
     *-----------------------------------------------------------------------*/
    graph->add_output(granulator);

    /*------------------------------------------------------------------------
     * Begin audio processing, and loop forever.
     *-----------------------------------------------------------------------*/
    graph->start();
    graph->wait();
}

