#include "env.h"
#include "../graph.h"

namespace libsignal
{

ASR::ASR(NodeRef attack, NodeRef sustain, NodeRef release, NodeRef clock) :
	attack(attack), sustain(sustain), release(release), clock(clock)
{
	this->phase = 0.0;
	this->clock_last = 0.0;

	this->name = "env-asr";
	this->add_param("clock", this->clock);
	this->add_param("attack", this->attack);
	this->add_param("sustain", this->sustain);
	this->add_param("release", this->release);
}

void ASR::trigger()
{
	this->phase = 0.0;
}

void ASR::process(sample **out, int num_frames)
{
	sample rv;

	for (int frame = 0; frame < num_frames; frame++)
	{
		if (this->clock)
		{
			sample clock_value = this->clock->out[0][frame];
			if (clock_value > this->clock_last)
				this->trigger();
			this->clock_last = clock_value;
		}

		float attack = this->attack->out[0][frame];
		float sustain = this->sustain->out[0][frame];
		float release = this->release->out[0][frame];

		if (this->phase < attack)
		{
			rv = (this->phase / attack);
		}
		else if (this->phase <= attack + sustain)
		{
			rv = 1.0;
		}
		else if (this->phase < attack + sustain + release)
		{
			rv = 1.0 - (this->phase - (attack + sustain)) / release;
		}
		else
		{
			rv = 0.0;
		}

		this->phase += 1.0 / this->graph->sample_rate;

		for (int channel = 0; channel < this->num_output_channels; channel++)
		{
			out[channel][frame] = rv;
		}
	}
}

}
