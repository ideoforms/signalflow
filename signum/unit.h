#pragma once

#include "constants.h"
#include "buffer.h"

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
			UnitRefT(Unit *ptr); // : std::shared_ptr<Unit>(ptr) {}
			UnitRefT(double x); // : std::shared_ptr<Unit>(new Unit(x)) {}

			UnitRefT operator* (UnitRefT other);
			UnitRefT operator* (double constant);
			sample operator[] (int index);
	};

	typedef UnitRefT <Unit> UnitRef;


	class Unit
	{
		public:

			Unit();
			Unit(double x);

			virtual void next(int count);
			virtual sample next();
			virtual void route(UnitRef other);
			virtual void add_input(UnitRef other);
			virtual void add_param(std::string name, UnitRef &param);
			virtual void set_param(std::string name, UnitRef param);

			Buffer *output;

			op::Multiply operator* (UnitRef other);
			op::Multiply operator* (sample value);

			std::vector <UnitRef> inputs;
			std::unordered_map <std::string, UnitRef *> params;
	};

}
