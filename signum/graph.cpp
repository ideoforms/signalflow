#include "graph.h"
#include "unit.h"
#include "core.h"

#include "io/output/abstract.h"
#include "io/output/soundio.h"
#include "io/output/ios.h"

#include <unistd.h>

namespace signum
{
	Graph::Graph()
	{
		signum_init();
        
		this->output = new AudioOut(this);
		this->sample_rate = 44100.0;
	}

	void Graph::run()
	{
		while (true) { usleep(100000); }
	}

	void Graph::pull_input(const UnitRef &unit, int num_frames)
	{
		// Must pull our input's outputs first,
		// or pops will be caused in the output (something to do with phase?)
		// std::cout << "pull_input: " << unit << std::endl;
		// std::cout << "pull_input: " << unit->name << std::endl;

		for (auto input_unit : unit->inputs)
		{
			if (input_unit)
				this->pull_input(input_unit, num_frames);
		}

		for (auto param : unit->params)
		{
			UnitRef param_unit = *(param.second);
			if (param_unit)
				this->pull_input(param_unit, num_frames);
			// else
			//	fprintf(stderr, "Unit %s has a null param input (parameter: %s)\n", unit->name.c_str(), param.first.c_str());
		}
		unit->next(unit->out, num_frames);
	}

	void Graph::pull_input(int num_frames)
	{
		this->pull_input(this->output, num_frames);
	}

	UnitRef Graph::addUnit(Unit *unit)
	{
		return std::shared_ptr<Unit>(unit);
	}
}
