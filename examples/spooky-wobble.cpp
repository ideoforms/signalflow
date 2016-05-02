#include <signal/signal.h>

#include <unistd.h>
#include <stdlib.h>

using namespace libsignal;

int main()
{
	Graph *graph = new Graph();

	Buffer *buffer = new Buffer("audio/gliss.aif");
	NodeRef sampler = new Sampler(buffer, new Noise(0.3, true), true);
	NodeRef sampler_pan = new Pan(2, sampler, 0.25);
	graph->add_output(sampler_pan);

	NodeRef noise = new Noise(50, true);
	NodeRef noise_pan = new Pan(2, noise, 0.75);
	graph->add_output(noise_pan);

    NodeRef dust = new Dust(1);
    NodeRef dust_pan = new Pan(2, dust, 0.5);
	graph->add_output(dust_pan);

	while (true)
	{
		usleep(100000 * (1 + rand() % 4));
	}
}
