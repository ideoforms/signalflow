#include "unit.h"

#include "op/multiply.h"
#include "gen/constant.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

namespace signum
{

Unit::Unit()
{
	this->output = new RingBuffer <sample> (44100);
	this->channels_in = N_CHANNELS;
	this->channels_out = N_CHANNELS;

	printf("Unit constructor called\n");
}

void Unit::next(frame in, frame out)
{
	// Basic next() loop assumes we are N-in, N-out.
	// TODO: Assert channel config makes sense? (> 0)
	
	for (int i = 0; i < this->channels_out; i++)
	{
		out[i] = this->next();
	}
}

void Unit::next(int count)
{
	for (int i = 0; i < count; i++)
		this->output->append(this->next());
}

sample Unit::next()
{
	printf("Unit::next (should never be called)\n");
	exit(1);
}

void Unit::route(UnitRef other)
{
	other->add_input(std::shared_ptr <Unit>(this));
}

void Unit::add_input(UnitRef unit)
{
	// this->inputs.push_back(unit);
}

void Unit::add_param(std::string name, UnitRef &unit)
{
	this->params[name] = &unit;
}

void Unit::set_param(std::string name, UnitRef b)
{
	assert(this->params[name]);
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

template<>
sample UnitRef::operator[] (int index)
{
	return (*this)->output->get(index);
}

BinaryOpUnit::BinaryOpUnit(UnitRef a, UnitRef b) : Unit()
{
	this->add_input(a);
	this->add_input(b);
}

BinaryOpUnit::BinaryOpUnit(UnitRef a, sample b) : Unit()
{
	// inputA(a), inputB(new gen::Constant(b)) {};
	this->add_input(a);
	this->add_input(new gen::Constant(b));
}

void UnaryOpUnit::add_input(UnitRef unit)
{
	this->inputs.push_back(unit);
	this->input = unit;
}

}

