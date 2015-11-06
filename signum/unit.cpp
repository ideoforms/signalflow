#include "unit.h"

#include "op/multiply.h"

#include <stdio.h>
#include <stdlib.h>

namespace signum
{

Unit::Unit()
{
	printf("CREATING UNIT\n");
	this->output = new Buffer(1, 2048);
}

void Unit::next(int count)
{
	printf("Unit::next (should never be called)\n");
	exit(1);
}

Unit Unit::operator* (Unit &other)
{
	return op::Multiply(*this, other);
}

}

