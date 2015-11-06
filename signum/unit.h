#pragma once

#include "constants.h"
#include "buffer.h"

namespace signum
{

class Unit
{
	public:

		Unit();
		virtual void next(int count);
		Buffer *output;
};

}
