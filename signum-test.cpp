#include "signum/signum.h"

#include <unistd.h>
#include <stdlib.h>

using namespace signum;

int main()
{
	io::AudioOut output;
	// gen::SquareWave sine(440);

	// gen::Sampler sampler("gliss.aif");

	gen::Sampler *sampler = new gen::Sampler("gliss.aif");
	sampler->loop = true;
	// sampler->rate = new Scale(new gen::Noise(0.3, true), 0.5, 5);
	sampler->rate = new gen::Noise(0.3, true);

	// sampler->loop = true;

	// Unit *sine = new gen::Sine(0.3);

	// env::ASR env(0.01, 0.1, 0.05);

	// op::Multiply mult(sampler, env);
	// op::Multiply attenuated(mult, 0.1);

	// output.add_input(attenuated);
	
	// op::Multiply mult = sampler * env;
	// op::Multiply mult2 = mult * 0.1;

	// op::Multiply mult = sampler * sinewave;
	
	// Unit *mult = new op::Multiply(*sampler, *sine);
	// Unit *mult = &(sampler * sine);

	sampler->route(output);

	gen::Noise *noise = new gen::Noise(50, true);
	noise->route(output);

	while (true)
	{
		// env.trigger();
		// sampler->trigger();
		// printf("beep\n");
		usleep(100000 * (1 + rand() % 4));
		// sine.frequency = 200 + rand() % 1000;
	}
}
