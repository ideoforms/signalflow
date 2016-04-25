#include "trigger_noise.h"

#include "../util.h"

#include <limits>

namespace libsignal
{

TriggerNoise::TriggerNoise(NodeRef min, NodeRef max, NodeRef clock) : min(min), max(max), clock(clock)
{
	this->name = "trigger_noise";
	this->add_input("min", this->min);
	this->add_input("max", this->max);
	this->add_input("clock", this->clock);

	for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
		this->value[i] = std::numeric_limits<float>::max();
}

void TriggerNoise::trigger(std::string name, float value)
{
	for (int channel = 0; channel < SIGNAL_MAX_CHANNELS; channel++)
	{
		this->value[channel] = random_uniform(this->min->out[channel][0], this->max->out[channel][0]);
	}
}

void TriggerNoise::process(sample **out, int num_frames)
{
	for (int channel = 0; channel < this->num_output_channels; channel++)
	{
		if (this->value[channel] == std::numeric_limits<float>::max())
		{
			this->value[channel] = random_uniform(this->min->out[channel][0], this->max->out[channel][0]);
		}

		for (int frame = 0; frame < num_frames; frame++)
		{
			if (SIGNAL_CHECK_TRIGGER(clock, frame))
			{
				float min = this->min->out[channel][frame];
				float max = this->max->out[channel][frame];
				this->value[channel] = random_uniform(this->min->out[channel][frame], this->max->out[channel][frame]);
			}

			this->out[channel][frame] = this->value[channel];
		}
	}
}

}
