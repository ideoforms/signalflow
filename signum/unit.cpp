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

void Unit::add_param(std::string name, UnitRef &unit)
{
	this->params[name] = &unit;
}

void Unit::set_param(std::string name, UnitRef b)
{
	*(this->params[name]) = b;
}


template<>
UnitRef::UnitRefT() : std::shared_ptr<Unit>(nullptr) { }

template<>
UnitRef::UnitRefT(Unit *ptr) : std::shared_ptr<Unit>(ptr) { }

template<>
UnitRef::UnitRefT(double x) : std::shared_ptr<Unit>(new gen::Constant(x)) { }

template<>
UnitRef UnitRef::operator* (UnitRef other)
	{ return UnitRef(new op::Multiply(*this, other)); }

template<>
UnitRef UnitRef::operator* (double constant)
	{ return UnitRef(new op::Multiply(*this, constant)); }


}

