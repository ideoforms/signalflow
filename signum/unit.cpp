#include "unit.h"

#include <stdio.h>
#include <stdlib.h>

namespace signum
{

Unit::Unit()
{
	this->output = new Buffer(1, 2048);
}

void Unit::next(int count)
{
	printf("Unit::next (should never be called)\n");
	exit(1);
}


}
