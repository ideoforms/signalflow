#pragma once

#include "constants.h"
#include "buffer.h"

#include <vector>

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

	class Unit
	{
		public:

			Unit();

			virtual void next(int count);
			virtual sample next();
			virtual void route(Unit &other);
			virtual void add_input(Unit &other);

			Buffer *output;

			op::Multiply operator* (Unit& other);
			op::Multiply operator* (sample value);

			std::vector <Unit *> inputs;
	};

}
