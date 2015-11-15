#pragma once 

#include "unit.h"

#include <unistd.h>

namespace signum
{

	class Graph
	{
		public:
			Graph();
			void run()
			{
				while (true) { usleep(100000); }
			}
			UnitRef addUnit(Unit *unit)
			{
				return std::shared_ptr<Unit>(unit);
			}

			UnitRef input = nullptr;
			UnitRef output = nullptr;
	};
}
