#include "signum/signum.h"

using namespace signum;

int main()
{
	io::AudioOut output;
	gen::SquareWave square(440.0, 0.7);
	output.add_input(square);
	output.start();
}
