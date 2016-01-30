#pragma once 

#include "../unit.h"

namespace signum
{
	class Wavetable : public Unit
	{
	public:
		Wavetable(Buffer *table, UnitRef frequency, bool interpolate = true) :
			table(table), frequency(frequency), interpolate(interpolate)
		{
			this->phase = 0.0;
			this->name = "wavetable";

			this->add_param("frequency", this->frequency);
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int frame = 0; frame < num_frames; frame++)
			{
				float frequency = this->frequency->out[0][frame];
				int index = this->phase * this->table->num_frames;
				float rv = this->table->data[0][index];

				this->out[0][frame] = rv;

				this->phase += (frequency / this->graph->sample_rate);
				while (this->phase >= 1.0)
					this->phase -= 1.0;
			}
		}

		Buffer *table;
		UnitRef frequency;
		float phase;
		bool interpolate;

	};
}
