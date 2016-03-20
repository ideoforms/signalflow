#include "graph.h"
#include "unit.h"
#include "core.h"

#include "io/output/abstract.h"
#include "io/output/soundio.h"
#include "io/output/ios.h"

#include <unistd.h>

namespace libsignal
{
	Graph::Graph()
	{
		signal_init();
        
		this->output = new AudioOut(this);
		this->sample_rate = 44100.0;
	}

	void Graph::run()
	{
		while (true) { usleep(100000); }
	}

	void Graph::pull_input(const UnitRef &unit, int num_frames)
	{
		/*------------------------------------------------------------------------
		 * Must pull our inputs before we generate our own outputs.
		 *-----------------------------------------------------------------------*/

		signal_assert (unit->params.size() > 0 || unit->name == "constant" || unit->name == "audioout",
			"Unit %s has no registered params", unit->name.c_str());

		for (auto param : unit->params)

		{
			UnitRef param_unit = *(param.second);
			if (param_unit)
			{
				this->pull_input(param_unit, num_frames);

				/*------------------------------------------------------------------------
				 * Automatic upmix.
				 *
				 * If the input unit produces less channels than demanded, automatically
				 * up-mix its output by replicating the existing channels. This allows
				 * operations between multi-channel and mono-channel inputs to work
				 * seamlessly without any additional implementation within the node
				 * itself (for example, Multiply(new Sine(440), new Pan(2, ...)))
				 *
				 * A few units must prevent automatic up-mixing from happening.
				 * These include Multiplex and AudioOut.
				 *-----------------------------------------------------------------------*/
				if (param_unit->channels_out < unit->channels_in && !unit->no_input_automix)
				{
					signal_debug("Upmixing %s (%s wants %d channels, I only produce %d)", param_unit->name.c_str(),
						unit->name.c_str(), unit->channels_in, param_unit->channels_out);

					/*------------------------------------------------------------------------
					 * If we generate 2 channels but have 6 channels demanded, repeat
					 * them: [ 0, 1, 0, 1, 0, 1 ]
					 *-----------------------------------------------------------------------*/
					for (int out_channel_index = param_unit->channels_out;
					         out_channel_index < unit->channels_in;
					         out_channel_index ++)
					{
						int in_channel_index = out_channel_index % param_unit->channels_out;
						memcpy(param_unit->out[out_channel_index],
						       param_unit->out[in_channel_index],
						       num_frames * sizeof(sample));
					}
				}
			}
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
