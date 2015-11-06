#include "signum/signum.h"

#include <unistd.h>
#include <stdlib.h>

using namespace signum;

int main()
{
	io::AudioOut output;
	gen::SineWave sine(440);
	env::ASR env(0.01, 0.1, 0.05);

	op::Multiply mult(sine, env);
	op::Multiply attenuated(mult, 0.1);

	output.add_input(attenuated);

	while (true)
	{
		printf("beep\n");
		usleep(200000);
		sine.frequency = 200 + rand() % 1000;
		env.trigger();
	}
}
