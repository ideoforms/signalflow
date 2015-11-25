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

			void pull_input(UnitRef &unit, int num_frames)
			{
				// Must pull our input's outputs first,
				// or pops will be caused in the output (something to do with phase?)
				for (UnitRef input : unit->inputs)
				{
					this->pull_input(input, num_frames);
				}
				for (auto param : unit->params)
				{
					UnitRef param_unit = *(param.second);
					this->pull_input(param_unit, num_frames);
				}
				unit->next(unit->out, num_frames);
			}

			void pull_input(int num_frames)
			{
				this->pull_input(this->output, num_frames);
			}

			UnitRef addUnit(Unit *unit)
			{
				return std::shared_ptr<Unit>(unit);
			}

			UnitRef input = nullptr;
			UnitRef output = nullptr;
	};
}
