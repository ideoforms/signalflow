#include "granulator.h"

#include "constant.h"

namespace signum::gen
{

Granulator::Granulator(Buffer &buffer, Unit &clock, Unit &pos, float grain_length)
{
	this->buffer = &buffer;
	this->clock = &clock;
	this->pos = &pos;

	this->grain_length = grain_length;

	this->clock_last = 0.0;
}

void Granulator::next(int count)
{
	this->clock->next(count);
	this->pos->next(count);

	for (int i = 0; i < count; i++)
	{
		float pos = this->pos->output->data[0][i];

		sample clock_value = this->clock->output->data[0][i];
		if (clock_value > clock_last)
		{
			Grain *grain = new Grain(*buffer, pos * 44100.0, grain_length * 44100.0);
			this->grains.push_back(grain);
		}
		clock_last = clock_value;

		this->output->data[0][i] = 0.0;

		std::vector<Grain *>::iterator it;
		for (it = this->grains.begin(); it < this->grains.end(); )
		{
			Grain *grain = *it;

			if (!grain->finished())
			{
				int buffer_index = (grain->sample_start + grain->samples_done) % this->buffer->num_frames;
				sample s = this->buffer->data[0][(int) buffer_index];

				int half_grain_samples = grain->sample_length / 2;
				float amp;
				if (grain->samples_done <= half_grain_samples)
					amp = (float) grain->samples_done / half_grain_samples;
				else
					amp = 1.0 - (float) (grain->samples_done - half_grain_samples) / half_grain_samples;

				grain->samples_done++;
				this->output->data[0][i] += s * amp;

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
