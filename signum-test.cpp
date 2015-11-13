#include "signum/signum.h"

#include <unistd.h>
#include <stdlib.h>

using namespace signum;

Graph *graph = new Graph();

void sine_test()
{
	UnitRef sine = new gen::Sine(440.0);
	sine->route(graph->output);
}

void buffer_test()
{
	Buffer *buffer = new Buffer("gliss.aif");
	UnitRef dust = new rnd::Dust(1);
	UnitRef pos = new rnd::Noise(0.05, true, 0, buffer->duration);
	UnitRef granulator = new gen::Granulator(buffer, dust, pos, 0.3);
	UnitRef delay = new fx::Delay(granulator, 0.2, 0.5);

	delay->route(graph->output);
}

void spooky_wobble_test()
{
	Buffer *buffer = new Buffer("gliss.aif");
	UnitRef sampler = new gen::Sampler(buffer, 1, true);
	sampler->set_param("rate", new rnd::Noise(0.3, true));

	sampler->route(graph->output);

	UnitRef noise = new rnd::Noise(50, true);
	noise->route(graph->output);

	UnitRef dust = new rnd::Dust(1);
	dust->route(graph->output);

	graph->run();
}

int main()
{
	signum_init();
	spooky_wobble_test();
	// spooky_wobble_test();
	// sine_test();
	buffer_test();
	graph->run();
}

