#include "graph.h"
#include "signum.h"

#include "io/output.h"

namespace signum
{

Graph::Graph()
{
	signum_init();
	
	// input = new io::AudioIn();
	this->output = new io::AudioOut(this);
} 

};
