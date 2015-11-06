#include "unit.h"

#include "op/multiply.h"
#include "gen/constant.h"

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

void Unit::route(Unit &other)
{
	other.add_input(*this);
}

void Unit::add_input(Unit &unit)
{
	this->inputs.push_back(&unit);
}


op::Multiply Unit::operator* (Unit &other)
{
	return op::Multiply(*this, other);
}

op::Multiply Unit::operator* (sample value)
{
	return op::Multiply(*this, value);
}

}

