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

	this->grain_buffer_sample_start = 0;
	this->grain_samples_total = 0;
	this->grain_samples_done = 0;
}

void Granulator::next(int count)
{
	this->clock->next(count);
	this->pos->next(count);

	for (int i = 0; i < count; i++)
	{
		sample clock_value = this->clock->output->data[0][i];
		if (clock_value > clock_last)
		{
			printf("trigger\n");
			// Do we want the next() function just to request one sample
			// so we don't have to be indexing into our inputs?
			// 
			// This would imply being able to look into history too (sample -1, -2 ..)
			// 
			// Do we want a macro to get the first buffer of a unit?

			this->grain_buffer_sample_start = this->pos->output->data[0][i];
			this->grain_samples_done = 0;
			this->grain_samples_total = this->grain_length * 44100.0;

			// Grain grain(grain_buffer_sample_start, grain_samples_total);
			// this->grains.push_back(grain);
		}
		clock_last = clock_value;

		if (this->grain_samples_done < this->grain_samples_total)
		{
			int buffer_index = (this->grain_buffer_sample_start + this->grain_samples_done) % this->buffer->num_frames;
			sample s = this->buffer->data[0][(int) buffer_index];

			// TODO: Proper envelope handling.
			int half_grain_samples = this->grain_samples_total / 2;
			float amp;
			if (this->grain_samples_done <= half_grain_samples)
				amp = (float) this->grain_samples_done / half_grain_samples;
			else
				amp = 1.0 - (float) (this->grain_samples_done - half_grain_samples) / half_grain_samples;

			this->grain_samples_done++;
			this->output->data[0][i] = s * amp;
		}
		else
		{
			this->output->data[0][i] = 0.0;
		}
	}
}


}
