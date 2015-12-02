#include <signum/signum.h>

using namespace signum;

int main()
{
	Graph *graph = new Graph();

	Buffer *buffer = new EnvelopeBufferHanning(256);
	UnitRef freq = new rnd::Noise(2.0, true, 140, 800);
	freq = new op::RoundToScale(freq);
	UnitRef wavetable = new gen::Wavetable(buffer, freq * 2.0);
	UnitRef delay = new fx::Delay(wavetable, 0.1, 0.3);
	UnitRef attenuated = delay * 0.2;
	graph->output->add_input(attenuated);

	graph->run();
}

