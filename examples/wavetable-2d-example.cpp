/*------------------------------------------------------------------------
 * Wavetable example
 *
 * Demonstrates using the `Wavetable` oscillator to generate periodic
 * waveforms from a fixed audio buffer.
 *-----------------------------------------------------------------------*/
#include <signal/signal.h>
#include <math.h>

using namespace libsignal;

int main()
{
    AudioGraphRef graph = new AudioGraph();

    int wavetable_size = 4096;

    std::vector<std::vector<float>>wt(1);
    wt[0] = std::vector<float>(wavetable_size);
    for (int i = 0; i < wavetable_size; i++)
    {
        wt[0][i] = sin(i * M_PI * 2 / wavetable_size);
    }
    BufferRef bufferA = new Buffer(1, wavetable_size, wt);

    std::vector<std::vector<float>>wt2(1);
    wt2[0] = std::vector<float>(wavetable_size);
    for (int i = 0; i < wavetable_size; i++)
    {
        if (i < wavetable_size / 2)
            wt2[0][i] = -1;
        else
            wt2[0][i] = 1;
    }
    BufferRef bufferB = new Buffer(1, wavetable_size, wt2);
    BufferRef2D buffer2D = new InterpolatingBuffer2D(bufferA, bufferB);
    NodeRef morph = new Saw(2.0);
    morph = morph * 0.5 + 0.5;

    float freq = 50.0;
    NodeRef wavetable2D = new Wavetable2D(buffer2D, freq, 1.0);
    NodeRef delay = new Delay(wavetable2D, 0.1, 0.3);

    NodeRef pan = new Pan(2, delay);

    graph->add_output(pan);

    graph->start();
    graph->wait();
}

