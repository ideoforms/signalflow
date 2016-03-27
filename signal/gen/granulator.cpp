#include "granulator.h"

#include "constant.h"

namespace libsignal
{

Granulator::Granulator(Buffer *buffer, NodeRef clock, NodeRef pos, NodeRef grain_length) :
	buffer(buffer), pos(pos), clock(clock), grain_length(grain_length)
{
	this->name = "granulator";

	this->add_param("pos", this->pos);
	this->add_param("clock", this->clock);
	this->add_param("grain_length", this->grain_length);

	this->envelope = new EnvelopeBufferTriangle();
	this->add_buffer("envelope", &envelope);

	this->num_output_channels = 2;

	this->pan = 0.5;
	this->pan = 0.5;
	this->add_param("pan", this->pan);

	this->clock_last = 0.0;
}

void Granulator::set_spatialisation(int num_channels, NodeRef pan)
{
	this->num_output_channels = num_channels;
	this->pan = pan;
}

void Granulator::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		sample pos = this->pos->out[0][frame];
		sample clock_value = this->clock->out[0][frame];
		sample grain_length = this->grain_length->out[0][frame];
		sample pan = this->pan->out[0][frame];

		if (clock_value > clock_last)
		{
			Grain *grain = new Grain(buffer, pos * buffer->sample_rate, grain_length * buffer->sample_rate, pan);
			this->grains.push_back(grain);
		}
		clock_last = clock_value;

		for (int channel = 0; channel < this->num_output_channels; channel++)
			out[channel][frame] = 0.0;

		std::vector<Grain *>::iterator it;
		for (it = this->grains.begin(); it < this->grains.end(); )
		{
			Grain *grain = *it;
			if (!grain->finished())
			{
				/*------------------------------------------------------------------------
				 * Obtain the correct saple from the buffer.
				 * TODO: Handling of variable rate.
				 *-----------------------------------------------------------------------*/
				int buffer_index = (grain->sample_start + grain->samples_done) % this->buffer->num_frames;
				sample s = this->buffer->data[0][(int) buffer_index];

				/*------------------------------------------------------------------------
				 * Apply grain envelope.
				 *-----------------------------------------------------------------------*/
				float env_phase = (float) (grain->samples_done) / grain->sample_length;
				float amp = this->envelope->get_amplitude(env_phase);

				grain->samples_done++;

				/*------------------------------------------------------------------------
				 * Calculate pan.
				 * TODO: Handle >2 channels
				 *-----------------------------------------------------------------------*/
				float rv = s * amp;
				out[0][frame] += rv * (1.0 - grain->pan);
				out[1][frame] += rv * (grain->pan);

				it++;
			}
			else
			{
				delete grain;
				grains.erase(it);
			}
		}
	}
}


}
