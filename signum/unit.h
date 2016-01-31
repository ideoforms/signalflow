#pragma once

#include "constants.h"
#include "ringbuffer.h"
#include "registry.h"
#include "platform.h"
#include "buffer.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>


namespace signum
{

	/*------------------------------------------------------------------------
	 * Forward-declare our operator classes to avoid interdependencies.
	 *-----------------------------------------------------------------------*/
	class Multiply;
	class Add;
	class Subtract;
	class Divide;

	class Unit;
	class Graph;
	class UnitMonitor;

	/*------------------------------------------------------------------------
	 * Allows us to use a float (or direct unit ptr) in place of a UnitRef
	 * by specifying conversion constructors.
	 *-----------------------------------------------------------------------*/
	template<class T>
	class UnitRefT : public std::shared_ptr<T>
	{
		public:
			using std::shared_ptr<T>::shared_ptr;
			UnitRefT();
			UnitRefT(Unit *ptr);
			UnitRefT(double x);
			UnitRefT(int x);

			UnitRefT operator* (UnitRefT other);
			UnitRefT operator* (double constant);
			UnitRefT operator+ (UnitRefT other);
			UnitRefT operator+ (double constant);
			UnitRefT operator- (UnitRefT other);
			UnitRefT operator- (double constant);
			UnitRefT operator/ (UnitRefT other);
			UnitRefT operator/ (double constant);
			sample operator[] (int index);

			// UnitRefT operator= (const UnitRefT &other);

	};

	typedef UnitRefT <Unit> UnitRef;

	class Unit
	{

		public:

			Unit();
			Unit(double x);

			virtual void next(sample **out, int num_frames);

			/*------------------------------------------------------------------------
			 * Connect a new signal input to this unit. These connections form
			 * the overall signal graph.
			 *-----------------------------------------------------------------------*/
			virtual void add_input(UnitRef input) {}

			/*------------------------------------------------------------------------
			 * Called after add_input/route to update our routing ins/outs.
			 *-----------------------------------------------------------------------*/
			virtual void update_channels();

			/*------------------------------------------------------------------------
			 * Register parameters.
			 *-----------------------------------------------------------------------*/
			virtual void add_param(std::string name, UnitRef &param);
			virtual void set_param(std::string name, const UnitRef &param);

			/*------------------------------------------------------------------------
			 * Register buffer params.
			 *-----------------------------------------------------------------------*/
			virtual void add_buffer(std::string name, Buffer **buffer);
			virtual void set_buffer(std::string name, Buffer *buffer);

			/*------------------------------------------------------------------------
			 * TODO: Generic trigger method. Take named param for trigger ID?
			 *-----------------------------------------------------------------------*/
			virtual void trigger();

			/*------------------------------------------------------------------------
			 * Sets our output buffer to zero.
			 *-----------------------------------------------------------------------*/
			virtual void zero_output();

			/*------------------------------------------------------------------------
			 * Outputs the unit's value at a user-specified frequency.
			 *-----------------------------------------------------------------------*/
			virtual void poll(float frequency = 1.0, std::string label = "");
			UnitMonitor *monitor;

			/*------------------------------------------------------------------------
			 * Overloading operators allows us to write
			 * UnitRef foo = bar * 0.5;
			 *-----------------------------------------------------------------------*/
			Multiply operator* (UnitRef other);
			Multiply operator* (sample value);

			/*------------------------------------------------------------------------
			 * Human-readable name identifier [a-z0-9-]
			 *----------------------------------------------------------------------*/
			std::string name;

			/*------------------------------------------------------------------------
			 * Hash table of parameters: (name, pointer to UnitRef)
			 * Must be a pointer rather than the UnitRef itself as these
			 * params are actually pointers to struct fields (this->frequency).
			 *-----------------------------------------------------------------------*/
			std::unordered_map <std::string, UnitRef *> params;

			/*------------------------------------------------------------------------
			 * Buffers are distinct from parameters, pointing to a fixed
			 * area of sample storage that must be non-null.
			 *-----------------------------------------------------------------------*/
			std::unordered_map <std::string, Buffer **> buffers;

			/*------------------------------------------------------------------------
			 * Pointer to the Graph that this unit is a part of.
			 * Set automatically in constructor.
			 *-----------------------------------------------------------------------*/
			Graph *graph;

			/*------------------------------------------------------------------------
			 * Number of actual in/out channels. This should always reflect
			 * the number of audio channels allocated in our `out` buffer,
			 *-----------------------------------------------------------------------*/
			int channels_in;
			int channels_out;

			/*------------------------------------------------------------------------
			 * Number of preferred in/out channels. This is used to determine
			 * how audio should be up-mixed or down-mixed when passing signals
			 * between nodes.
			 *
			 * N-to-N nodes should use N_CHANNELS / N_CHANNELS
			 * 1-to-N nodes should use 1 / N_CHANNELS
			 *-----------------------------------------------------------------------*/
			int  min_input_channels,
			     max_input_channels,
			     min_output_channels,
			     max_output_channels;
			bool no_input_automix;

			/*------------------------------------------------------------------------
			 * Buffer containing this unit's output.
			 * TODO: Point this partway through a bigger frame buffer so that
			 * its history can be read for delay lines etc.
			 *-----------------------------------------------------------------------*/
			sample **out;

			/*------------------------------------------------------------------------
			 * Vector of input units.
			 *-----------------------------------------------------------------------*/
			// std::vector <UnitRef> inputs;

			/*------------------------------------------------------------------------
			 * Pointer to our outgoing connection.
			 *-----------------------------------------------------------------------*/
			UnitRef output = nullptr;

			/*------------------------------------------------------------------------
			 * A reference to the UnitRef shared_ptr pointing to this Unit.
			 * Necessary so that a unit can make outgoing/incoming connections to
			 * other UnitRefs, increasing its own shared_ptr's reference count.
			 *-----------------------------------------------------------------------*/
			UnitRef *ref;
	};

	class GeneratorUnit : public Unit
	{
		public:
			GeneratorUnit() : Unit()
			{
				this->channels_in = 0;
				this->channels_out = 1;
			}
	};

	class UnaryOpUnit : public Unit
	{
		public:
			UnaryOpUnit(UnitRef input = 0);

			UnitRef input;
	};

	class BinaryOpUnit : public Unit
	{
		public:
			BinaryOpUnit(UnitRef a = 0, UnitRef b = 0);

			UnitRef input0;
			UnitRef input1;
	};
}

#include "graph.h"
