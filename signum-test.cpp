#include "signum/signum.h"

#include <unistd.h>
#include <stdlib.h>

using namespace signum;

io::AudioOut output;

void sine_test()
{
	gen::Sine *sine = new gen::Sine(440.0);
	sine->route(output);
}

void buffer_test()
{
	Buffer *buffer = new Buffer("gliss.aif");
	Unit *dust = new rnd::Dust(1);
	Unit *pos = new rnd::Noise(0.05, true, 0, buffer->duration);
	Unit *granulator = new gen::Granulator(buffer, dust, pos, 0.3);
	fx::Delay *delay = new fx::Delay(granulator, 0.2, 0.5);

	delay->route(output);
}

void spooky_wobble_test()
{
    gen::Sampler *sampler = new gen::Sampler("gliss.aif");
    sampler->loop = true;
    sampler->rate = new rnd::Noise(0.3, true);

    sampler->route(output);

    rnd::Noise *noise = new rnd::Noise(50, true);
    noise->route(output);

    rnd::Dust *dust = new rnd::Dust(1);
    dust->route(output);

	output.start();
}

int main()
{
	signum_init();
	spooky_wobble_test();
	// spooky_wobble_test();
	// sine_test();
	buffer_test();
	output.start();
}

