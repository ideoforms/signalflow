#pragma once

#include "constants.h"
#include "ringbuffer.h"
#include "registry.h"

#include <string>
#include <vector>
#include <unordered_map>


namespace signum
{

	/*------------------------------------------------------------------------
	 * Forward-declare our operator classes to avoid interdependencies.
	 *-----------------------------------------------------------------------*/
	namespace op
	{
		class Multiply;
		class Add;
		class Subtract;
		class Divide;
	}

	class Unit;
	class Graph;


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
			 * Register inputs and parameters.
			 * This is necessary to register connections to form the signal graph.
			 * TODO: How can we enforce this? Should we abolish fields altogether?
			 *       sample *frequency = this->get_param_output("frequency");
			 *-----------------------------------------------------------------------*/
			virtual void route(const UnitRef &other);

			/*------------------------------------------------------------------------
			 * Register inputs and parameters.
			 * This is necessary to register connections to form the signal graph.
			 * TODO: How can we enforce this? Should we abolish fields altogether?
			 *       sample *frequency = this->get_param_output("frequency");
			 *-----------------------------------------------------------------------*/
			virtual void add_input(const UnitRef &input);
			virtual void add_param(std::string name, UnitRef &param);
			virtual void set_param(std::string name, const UnitRef &param);

			/*------------------------------------------------------------------------
			 * Overloading operators allows us to write
			 * UnitRef foo = bar * 0.5;
			 *-----------------------------------------------------------------------*/
			op::Multiply operator* (UnitRef other);
			op::Multiply operator* (sample value);

			/*------------------------------------------------------------------------
			 * General properties:
			 *  - human-readable name identifier [a-z0-9-]
			 *  - vector of parameters: (name, pointer to UnitRef)
			 *    - must be a pointer rather than the UnitRef itself as these
			 *      params are actually pointers to struct fields (this->frequency)
			 *  - vector of input units
			 *-----------------------------------------------------------------------*/
			std::string name;
			std::unordered_map <std::string, UnitRef *> params;
			// std::vector <UnitRef> inputs;

			/*------------------------------------------------------------------------
			 * Pointer to the Graph that this unit is a part of.
			 * Set automatically in constructor.
			 *-----------------------------------------------------------------------*/
			Graph *graph;

			/*------------------------------------------------------------------------
			 * Number of in/out channels.
			 *-----------------------------------------------------------------------*/
			int channels_in;
			int channels_out;

			/*------------------------------------------------------------------------
			 * Buffer containing this unit's output.
			 * TODO: Point this partway through a bigger frame buffer so that
			 * its history can be read for delay lines etc.
			 *-----------------------------------------------------------------------*/
			sample **out;

			std::vector <UnitRef> inputs;

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
			virtual void set_param(std::string name, const UnitRef &param);

			UnitRef input;
	};

	class BinaryOpUnit : public Unit
	{
		public:
			BinaryOpUnit(UnitRef a = 0, UnitRef b = 0);
			virtual void set_param(std::string name, const UnitRef &param);

			UnitRef input0;
			UnitRef input1;
	};
}
