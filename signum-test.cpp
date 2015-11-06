#include "signum/signum.h"

using namespace signum;

int main()
{
	io::AudioOut output;
	gen::SineWave a(440);
	env::ASR env(0.01, 0.5, 1.0);
	op::Multiply mult(a, env);

	output.add_input(mult);
	output.start();
}
