#include "signum/signum.h"

#include <unistd.h>
#include <stdlib.h>

using namespace signum;

int main()
{
	signum_init();

	io::AudioOut output;

	Buffer *buffer = new Buffer("gliss.aif");
	rnd::Dust *dust = new rnd::Dust(25);
	rnd::Noise *pos = new rnd::Noise(0.05, true, 0, buffer->duration);

	gen::Granulator *granulator = new gen::Granulator(*buffer, *dust, *pos, 0.3);

	granulator->route(output);

	output.start();
}
