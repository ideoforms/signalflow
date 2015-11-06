#include "signum/signum.h"

using namespace signum;

int main()
{
	io::AudioOut output = io::AudioOut();
	gen::SineWave sine = gen::SineWave(440.0);
	output.add_input(sine);
	output.start();
}
