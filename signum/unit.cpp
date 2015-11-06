#include "unit.h"

#include <stdio.h>
#include <stdlib.h>

namespace signum
{

void Unit::next(Buffer &buffer, int count)
{
	printf("Unit::next (should never be called)\n");
	exit(1);
}

int connect(Unit output)
{
	// output.add_input(*this);
	return 0;
}

}
