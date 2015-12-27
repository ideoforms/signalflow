#include "signum/signum.h"

#include <unistd.h>
#include <stdlib.h>

using namespace signum;

int main()
{
	Graph *graph = new Graph();

	Buffer *buffer = new Buffer("audio/gliss.aif");
	UnitRef sampler = new Sampler(buffer, new Noise(0.3, true), true);
	UnitRef sampler_pan = new Pan(2, sampler, 0.25);
	graph->output->add_input(sampler_pan);

	UnitRef noise = new Noise(50, true);
	UnitRef noise_pan = new Pan(2, noise, 0.75);
	graph->output->add_input(noise_pan);

    UnitRef dust = new Dust(1);
    UnitRef dust_pan = new Pan(2, dust, 0.5);
	graph->output->add_input(dust_pan);

	while (true)
	{
		usleep(100000 * (1 + rand() % 4));
	}
}
