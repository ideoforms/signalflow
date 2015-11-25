#include "graph.h"
#include "signum.h"
#include "io/output.h"

namespace signum
{
	Graph::Graph()
	{
		signum_init();
		this->output = new io::AudioOut(this);
	}

	void Graph::run()
	{
		while (true) { usleep(100000); }
	}

	void Graph::pull_input(UnitRef &unit, int num_frames)
	{
		// Must pull our input's outputs first,
		// or pops will be caused in the output (something to do with phase?)
		for (UnitRef input : unit->inputs)
		{
			if (!input)
				fprintf(stderr, "Unit %s has a null input\n", unit->name.c_str());
			this->pull_input(input, num_frames);
		}
		for (auto param : unit->params)
		{
			UnitRef param_unit = *(param.second);
			if (param_unit)
				this->pull_input(param_unit, num_frames);
			else
				fprintf(stderr, "Unit %s has a null param input (parameter: %s)\n", unit->name.c_str(), param.first.c_str());
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
