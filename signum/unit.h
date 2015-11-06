#pragma once

#include "constants.h"
#include "buffer.h"

namespace signum
{

class Unit
{
	public:

		virtual void next(Buffer &buffer, int count);
		int connect(Unit output);
};

}
