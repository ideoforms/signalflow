#include "unit.h"

#include "op/multiply.h"
#include "gen/constant.h"
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

extern signum::Graph * shared_graph;

namespace signum
{

Unit::Unit()
{
	// this->out = new RingBuffer <sample> (44100);
	this->graph = shared_graph;
	this->out = (sample **) malloc(32 * sizeof(float*));
	for (int i = 0; i < 32; i++)
		this->out[i] = (sample *) malloc(44100 * sizeof(float));

	this->channels_in = N_CHANNELS;
	this->channels_out = N_CHANNELS;
	this->channels_in = 1;
	this->channels_out = 1;
}

void Unit::next(sample **out, int num_frames)
{
	// Basic next() loop assumes we are N-in, N-out.
	// TODO: Assert channel config makes sense? (> 0)
	
	printf("Unit::next (should never be called)\n");
	exit(1);
}

void Unit::route(UnitRef other)
{
	// other->add_input(std::shared_ptr <Unit>(this));
	// other->add_input(std::shared_ptr <Unit>(this));
	other->add_input(*(this->ref));
}

void Unit::add_input(UnitRef unit)
{
	this->inputs.push_back(unit);
}

void Unit::add_param(std::string name, UnitRef &unit)
{
	this->params[name] = &unit;
}

void Unit::set_param(std::string name, UnitRef unit)
{
	assert(this->params[name]);
	*(this->params[name]) = unit;
}


template<>
UnitRef::UnitRefT() : std::shared_ptr<Unit>(nullptr) { }

template<>
UnitRef::UnitRefT(Unit *ptr) : std::shared_ptr<Unit>(ptr)
{
	ptr->ref = this;
}

template<>
UnitRef::UnitRefT(double x) : std::shared_ptr<Unit>(new gen::Constant(x))
{
	(*this)->ref = this;
}

template<>
UnitRef UnitRef::operator* (UnitRef other)
	{ return UnitRef(new op::Multiply(*this, other)); }

template<>
UnitRef UnitRef::operator* (double constant)
	{ return UnitRef(new op::Multiply(*this, constant)); }

template<>
sample UnitRef::operator[] (int index)
{
	// unused?
	return (*this)->out[0][index];
}

BinaryOpUnit::BinaryOpUnit(UnitRef a, UnitRef b) : Unit()
{
	this->add_input(a);
	this->add_input(b);
}

void UnaryOpUnit::add_input(UnitRef unit)
{
	this->inputs.push_back(unit);
	this->input = unit;
}

}

