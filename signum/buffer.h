#pragma once 

#include "constants.h"

#include <math.h>

#define SIGNUM_ENVELOPE_BUFFER_LENGTH 1024
#define SIGNUM_ENVELOPE_BUFFER_HALF_LENGTH (SIGNUM_ENVELOPE_BUFFER_LENGTH / 2)

namespace signum
{
	class Buffer
	{
	public:
		Buffer(int num_channels, int num_frames);
		Buffer(const char *filename);

		void open(const char *filename);

		// TODO: Should we assert a buffer is mono before indexing?
		sample get(int index) { return this->data[0][index]; }
		sample operator [](int index){ return this->get(index); }

		float sample_rate;
		int num_channels;
		int num_frames;
		sample **data;

		float duration;

		sample get_amplitude(float norm_index)
		{
			/*-------------------------------------------------------------------------
			 * TODO: Interpolate
			 * TODO: Should we be more polite about indexes outside of [0,1]?
			 *-----------------------------------------------------------------------*/
			int index = norm_index * this->num_frames;
			return this->data[0][index];
		}
	};

	/*-------------------------------------------------------------------------
	 * An EnvelopeBuffer is a mono buffer with a fixed number of samples,
	 * which can be sampled at a position [0,1] to give an amplitude value.
	 * Mostly intended to be subclassed.
	 *-----------------------------------------------------------------------*/
	class EnvelopeBuffer : public Buffer
	{
		public:
			EnvelopeBuffer(int length = SIGNUM_ENVELOPE_BUFFER_LENGTH) : Buffer(1, length)
			{
				/*-------------------------------------------------------------------------
				 * Initialise to a flat envelope at maximum amplitude.
				 *-----------------------------------------------------------------------*/
				for (int x = 0; x < length; x++)
					this->data[0][x] = 1.0;
			}

	};

	class EnvelopeBufferTriangle : public EnvelopeBuffer
	{
		public:
			EnvelopeBufferTriangle(int length = SIGNUM_ENVELOPE_BUFFER_LENGTH) : EnvelopeBuffer(length)
			{
				for (int x = 0; x < length / 2; x++)
					this->data[0][x] = (float) x / (length / 2);
				for (int x = 0; x < length / 2; x++)
					this->data[0][(length / 2) + x] = 1.0 - (float) x / (length / 2);
			}
	};

	class EnvelopeBufferLinearDecay : public EnvelopeBuffer
	{
		public:
			EnvelopeBufferLinearDecay(int length = SIGNUM_ENVELOPE_BUFFER_LENGTH) : EnvelopeBuffer(length)
			{
				for (int x = 0; x < length; x++)
					this->data[0][x] = 1.0 - (float) x / length;
			}
	};

	class EnvelopeBufferHanning : public EnvelopeBuffer
	{
		public:
			EnvelopeBufferHanning(int length = SIGNUM_ENVELOPE_BUFFER_LENGTH) : EnvelopeBuffer(length)
			{
				for (int x = 0; x < length; x++)
				{
					this->data[0][x] = 0.5 * (1.0 - cos(2 * M_PI * x / (length - 1)));
				}
			}
	};

}

