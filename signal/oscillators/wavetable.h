#pragma once 

#include "../node.h"

namespace libsignal
{
	class Wavetable : public Node
	{
	public:
		Wavetable(Buffer *table = NULL, NodeRef frequency = 440, bool interpolate = true) :
			table(table), frequency(frequency), interpolate(interpolate)
		{
			this->name = "wavetable";
			this->phase = 0.0;

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
		NodeRef frequency;
		float phase;
		bool interpolate;

	};

	REGISTER(Wavetable, "wavetable");
}
