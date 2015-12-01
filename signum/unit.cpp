#include "unit.h"

#include "op/multiply.h"
#include "op/add.h"
#include "op/subtract.h"
#include "op/divide.h"

#include "gen/constant.h"

#include "graph.h"
#include "util.h"

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
	this->ref = NULL;
}

void Unit::next(sample **out, int num_frames)
{
	// Basic next() loop assumes we are N-in, N-out.
	// TODO: Assert channel config makes sense? (> 0)
	
	printf("Unit::next (should never be called)\n");
	exit(1);
}

void Unit::add_input(const UnitRef &other)
{
}

void Unit::route(const UnitRef &other)
{
	// other->add_input(std::shared_ptr <Unit>(this));
	// other->add_input(std::shared_ptr <Unit>(this));

	/*------------------------------------------------------------------------
	 * Care required here -- if we naively do add_input(*(this->ref)), the
	 * shared_ptr is copied which breaks things (see TODO below).
	 * Explicitly cast to a const ref.
	 *-----------------------------------------------------------------------*/
	printf("ROUTE NO LONGER IMPLEMENTED\n");
	// const UnitRef &r = *(this->ref);
	// other->add_input(r);
}

void Unit::add_param(std::string name, UnitRef &unit)
{
	this->params[name] = &unit;
}

// TODO: Assignment operator breaks our paradigm as (I think) we need 
// to update the new object's 'ref' pointer to its shared_ptr container...
// This might be bad practice. 
/*
template<>
UnitRef UnitRef::operator= (const UnitRef &other)
{
	printf("UNITREF ASSIGN, HERE BE DRAGONS\n");
	// if (this != other)
	//	(*this)->ref = other->ref;
	return *this;
}
*/



void Unit::set_param(std::string name, const UnitRef &unit)
{
	if (!this->params[name])
	{
		fprintf(stderr, "Unit %s has no such param: %s\n", this->name.c_str(), name.c_str());
		exit(1);
	}

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
UnitRef::UnitRefT(int x) : std::shared_ptr<Unit>(new gen::Constant((float) x))
{
	(*this)->ref = this;
}


/*------------------------------------------------------------------------
 * Don't explicitly cast to UnitRef here or bad things happen
 * (shared_ptrs freed too early -- causing SIGSEGV when doing
 * sine * 0.25)
 *-----------------------------------------------------------------------*/
template<>
UnitRef UnitRef::operator* (UnitRef other)
	{ return new op::Multiply(*this, other); }

template<>
UnitRef UnitRef::operator* (double constant)
	{ return new op::Multiply(*this, constant); }

template<>
UnitRef UnitRef::operator+ (UnitRef other)
	{ return new op::Add(*this, other); }

template<>
UnitRef UnitRef::operator+ (double constant)
	{ return new op::Add(*this, constant); }

template<>
UnitRef UnitRef::operator- (UnitRef other)
	{ return new op::Subtract(*this, other); }

template<>
UnitRef UnitRef::operator- (double constant)
	{ return new op::Subtract(*this, constant); }

template<>
UnitRef UnitRef::operator/ (UnitRef other)
	{ return new op::Divide(*this, other); }

template<>
UnitRef UnitRef::operator/ (double constant)
	{ return new op::Divide(*this, constant); }

template<>
sample UnitRef::operator[] (int index)
{
	// unused?
	return (*this)->out[0][index];
}

BinaryOpUnit::BinaryOpUnit(UnitRef a, UnitRef b) : Unit()
{
	this->input0 = a;
	this->input1 = b;

	this->add_param("input0", this->input0);
	this->add_param("input1", this->input1);

	// upmixing
	this->channels_out = MAX(input0->channels_out, input1->channels_out);
}


void BinaryOpUnit::set_param(std::string name, const UnitRef &unit)
{
	Unit::set_param(name, unit);

	// upmixing
	this->channels_out = MAX(input0->channels_out, input1->channels_out);
}


UnaryOpUnit::UnaryOpUnit(UnitRef a) : Unit()
{
	this->input = a;

	this->add_param("input0", this->input);

	// upmixing
	this->channels_out = this->input->channels_out;
}

void UnaryOpUnit::set_param(std::string name, const UnitRef &unit)
{
	Unit::set_param(name, unit);

	// upmixing
	this->channels_out = this->input->channels_out;
}

}

