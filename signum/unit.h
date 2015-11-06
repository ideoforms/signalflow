#pragma once

#include "constants.h"
#include "buffer.h"

namespace signum
{

class Multiply;

class Unit
{
	public:

		Unit();
		virtual void next(int count);
		Buffer *output;

		Unit operator* (Unit& other);
		Unit operator* (sample value);
};

}
