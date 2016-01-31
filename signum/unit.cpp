#include "unit.h"

#include "op/multiply.h"
#include "op/add.h"
#include "op/subtract.h"
#include "op/divide.h"

#include "gen/constant.h"

#include "core.h"
#include "graph.h"
#include "util.h"
#include "monitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>


namespace signum
{
    
extern Graph *shared_graph;

Unit::Unit()
{
	this->graph = shared_graph;
	this->out = (sample **) malloc(SIGNUM_MAX_CHANNELS * sizeof(float*));
	for (int i = 0; i < SIGNUM_MAX_CHANNELS; i++)
		this->out[i] = (sample *) malloc(44100 * sizeof(float));

	this->min_input_channels = N_CHANNELS;
	this->max_input_channels = N_CHANNELS;
	this->min_output_channels = N_CHANNELS;
	this->max_output_channels = N_CHANNELS;

	this->channels_in = 1;
	this->channels_out = 1;

	this->no_input_automix = false;

	this->ref = NULL;
	this->monitor = NULL;
}

void Unit::next(sample **out, int num_frames)
{
	// Basic next() loop assumes we are N-in, N-out.
	// TODO: Assert channel config makes sense? (> 0)

	throw std::runtime_error("Unit::next (should never be called)");
}

void Unit::update_channels()
{
	if (this->min_input_channels == N_CHANNELS)
	{
		int max_channels = 0;
		for (auto param : this->params)
		{
			UnitRef *ptr = param.second;
			// A param may be registered but not yet set
			if (!ptr || !*ptr)
				continue;
			std::string param_name = param.first;
			// signum_debug("%s: update_channels (%s)", this->name.c_str(), param_name.c_str());

			UnitRef input = *ptr;
			if (input->channels_out > max_channels)
				max_channels = input->channels_out;
		}

		signum_debug("Unit %s set num_out_channels to %d", this->name.c_str(), max_channels);
		this->channels_in = max_channels;
		this->channels_out = max_channels;
	}
}

void Unit::add_param(std::string name, UnitRef &unit)
{
	this->params[name] = &unit;
	this->update_channels();

	if (unit)
		unit->update_channels();
}

void Unit::set_param(std::string name, const UnitRef &unit)
{
	if (this->params.find(name) == this->params.end())
		throw std::runtime_error("Unit " + this->name + " has no such param: " + name);

	*(this->params[name]) = unit;
	this->update_channels();
	unit->update_channels();
}



void Unit::add_buffer(std::string name, Buffer **buffer)
{
	this->buffers[name] = buffer;
}

void Unit::set_buffer(std::string name, Buffer *buffer)
{
	if (!this->buffers[name])
		throw std::runtime_error("Unit " + this->name + " has no such buffer: " + name);

	*(this->buffers[name]) = buffer;
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

void Unit::zero_output()
{
	for (int i = 0; i < this->channels_out; i++)
		memset(this->out[i], 0, 44100 * sizeof(sample));
}

void Unit::trigger()
{
	// Placeholder
}

void Unit::poll(float frequency, std::string label)
{
	this->monitor = new UnitMonitor(this, label, frequency); 
	this->monitor->start();
}

/*------------------------------------------------------------------------
 * Default constructors. 
 *-----------------------------------------------------------------------*/

template<>
UnitRef::UnitRefT() : std::shared_ptr<Unit>(nullptr) { }

template<>
UnitRef::UnitRefT(Unit *ptr) : std::shared_ptr<Unit>(ptr)
	{ ptr->ref = this; }

template<>
UnitRef::UnitRefT(double x) : std::shared_ptr<Unit>(new Constant(x))
	{ (*this)->ref = this; }

template<>
UnitRef::UnitRefT(int x) : std::shared_ptr<Unit>(new Constant((float) x))
	{ (*this)->ref = this; }



/*------------------------------------------------------------------------
 * Don't explicitly cast to UnitRef here or bad things happen
 * (shared_ptrs freed too early -- causing SIGSEGV when doing
 * sine * 0.25)
 *-----------------------------------------------------------------------*/
template<>
UnitRef UnitRef::operator* (UnitRef other)
	{ return new Multiply(*this, other); }

template<>
UnitRef UnitRef::operator* (double constant)
	{ return new Multiply(*this, constant); }

template<>
UnitRef UnitRef::operator+ (UnitRef other)
	{ return new Add(*this, other); }

template<>
UnitRef UnitRef::operator+ (double constant)
	{ return new Add(*this, constant); }

template<>
UnitRef UnitRef::operator- (UnitRef other)
	{ return new Subtract(*this, other); }

template<>
UnitRef UnitRef::operator- (double constant)
	{ return new Subtract(*this, constant); }

template<>
UnitRef UnitRef::operator/ (UnitRef other)
	{ return new Divide(*this, other); }

template<>
UnitRef UnitRef::operator/ (double constant)
	{ return new Divide(*this, constant); }

template<>
sample UnitRef::operator[] (int index)
{
	// unused?
	return (*this)->out[0][index];
}

BinaryOpUnit::BinaryOpUnit(UnitRef a, UnitRef b) : Unit(), input0(a), input1(b)
{
	this->add_param("input0", this->input0);
	this->add_param("input1", this->input1);
}

UnaryOpUnit::UnaryOpUnit(UnitRef a) : Unit(), input(a)
{
	// TODO: Do this add_param automatically in add_input, with indexically-named
	//       inputs?
	this->add_param("input", this->input);
}

}

