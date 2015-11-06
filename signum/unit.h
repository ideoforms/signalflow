#pragma once

#include "constants.h"
#include "buffer.h"

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
			Buffer *output;

			op::Multiply operator* (Unit& other);
			op::Multiply operator* (sample value);
	};

}
