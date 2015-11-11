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

sample Unit::next()
{
	this->next(1);
	return this->output->data[0][0];
}

void Unit::route(UnitRef other)
{
	other->add_input(std::shared_ptr <Unit>(this));
}

void Unit::add_input(UnitRef unit)
{
	this->inputs.push_back(unit);
	printf("%p adding input (now got %ld)\n", this, this->inputs.size());
}


op::Multiply Unit::operator* (Unit &other)
{
	return op::Multiply(*this, other);
}

op::Multiply Unit::operator* (sample value)
{
	return op::Multiply(*this, value);
}

template<>
UnitRef::UnitRefT(Unit *ptr) : std::shared_ptr<Unit>(ptr) { }

template<>
UnitRef::UnitRefT(double x) : std::shared_ptr<Unit>(new gen::Constant(x)) {}

}

