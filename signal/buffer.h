#pragma once 

#include "constants.h"
#include "util.h"

#include <math.h>

#define SIGNAL_ENVELOPE_BUFFER_LENGTH 1024
#define SIGNAL_ENVELOPE_BUFFER_HALF_LENGTH (SIGNAL_ENVELOPE_BUFFER_LENGTH / 2)

namespace libsignal
{
	class Buffer
	{
	public:
		Buffer(int num_channels, int num_frames);
		Buffer(const char *filename);

		void open(const char *filename);

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
			EnvelopeBuffer(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH) : Buffer(1, length)
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
			EnvelopeBufferTriangle(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH) : EnvelopeBuffer(length)
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
			EnvelopeBufferLinearDecay(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH) : EnvelopeBuffer(length)
			{
				for (int x = 0; x < length; x++)
					this->data[0][x] = 1.0 - (float) x / length;
			}
	};

	class EnvelopeBufferHanning : public EnvelopeBuffer
	{
		public:
			EnvelopeBufferHanning(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH) : EnvelopeBuffer(length)
			{
				for (int x = 0; x < length; x++)
				{
					this->data[0][x] = 0.5 * (1.0 - cos(2 * M_PI * x / (length - 1)));
				}
			}
	};

	/*-------------------------------------------------------------------------
	 * A WaveShaperBuffer is a mono buffer with a fixed number of samples
	 * that can be sampled at a position [-1,1] to give an amplitude mapping
	 * value, equal to the result of the shaper's transfer function for
	 * that value.
	 *-----------------------------------------------------------------------*/
	class WaveShaperBuffer : public Buffer
	{
		public:
			WaveShaperBuffer(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH) : Buffer(1, length)
			{
				/*-------------------------------------------------------------------------
				 * Initialise to a flat envelope at maximum amplitude.
				 *-----------------------------------------------------------------------*/
				for (int x = 0; x < length; x++)
				{
					float mapped = map(x, 0, length - 1, -1, 1);
					this->data[0][x] = mapped;
				}
			}
	};
}
