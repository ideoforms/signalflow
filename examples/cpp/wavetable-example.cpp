/*------------------------------------------------------------------------
 * Wavetable example
 *
 * Demonstrates using the `Wavetable` oscillator to generate periodic
 * waveforms from a fixed audio buffer.
 *-----------------------------------------------------------------------*/
#include <math.h>
#include <signalflow/signalflow.h>

using namespace signalflow;

int main()
{
    AudioGraphRef graph = new AudioGraph();

    NodeRef freq = new WhiteNoise(2.0, true, 140, 800);
    freq = new RoundToScale(freq);

    int wavetable_size = 4096;
    std::vector<std::vector<float>> wt(1);
    wt[0] = std::vector<float>(wavetable_size);
    for (int i = 0; i < wavetable_size; i++)
    {
        wt[0][i] = sin(i * M_PI * 2 / wavetable_size);
    }
    BufferRef buffer = new Buffer(1, wavetable_size, wt);
    NodeRef wavetable = new Wavetable(buffer, freq * 2.0);
    NodeRef delay = new CombDelay(wavetable, 0.1, 0.3);

    NodeRef pan = new LinearPanner(2, delay);

    NodeRef attenuated = pan * 0.2;
    graph->play(attenuated);

    graph->start();
    graph->wait();
}
