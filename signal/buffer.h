#pragma once 

/**-------------------------------------------------------------------------
 * @file buffer.h
 * @brief Buffer and its subclasses store an array of sample values.
 *-----------------------------------------------------------------------*/

#include "constants.h"
#include "util.h"

#include <math.h>

#define SIGNAL_ENVELOPE_BUFFER_LENGTH 1024
#define SIGNAL_ENVELOPE_BUFFER_HALF_LENGTH (SIGNAL_ENVELOPE_BUFFER_LENGTH / 2)

typedef enum
{
	SIGNAL_INTERPOLATE_NONE,
	SIGNAL_INTERPOLATE_LINEAR
} signal_interpolate_t;

/**------------------------------------------------------------------------
 * Typedef for a sample -> sample transfer function.
 * Convenient for lambda-based features.
 *------------------------------------------------------------------------*/
typedef sample(*transfer_fn)(sample);

namespace libsignal
{
	class Buffer
	{
	public:
		Buffer(int num_channels, int num_frames);
		Buffer(const char *filename);

		void open(const char *filename);
		void save(const char *filename);

		float sample_rate;
		int num_channels;
		int num_frames;
		sample **data;

		float duration;
		signal_interpolate_t interpolate;

		/**------------------------------------------------------------------------
		 * Map a frame index to an offset in the buffer's native range.
		 *------------------------------------------------------------------------*/
		virtual double frame_to_offset(double frame)
		{
			return (double) frame;
		}

		/**------------------------------------------------------------------------
		 * Map an offset to a frame value.
		 *------------------------------------------------------------------------*/
		virtual double offset_to_frame(double offset)
		{
			return (double) offset;
		}

		/**------------------------------------------------------------------------
		 * @param frame The absolute frame value to retrieve.
		 * @return The raw value stored within that frame.
		 *------------------------------------------------------------------------*/
		sample get_frame(double frame)
		{
			// frame = clip(frame, 0, this->num_frames - 1);
			if (this->interpolate == SIGNAL_INTERPOLATE_LINEAR)
			{
				double frame_frac = (frame - (int) frame);
				sample rv = ((1.0 - frame_frac) * this->data[0][(int) frame]) + (frame_frac * this->data[0][(int) ceil(frame)]);
				return rv;
			}
			else
			{
				return this->data[0][(int) frame];
			}
		}

		/**------------------------------------------------------------------------
		 * @param index A sample offset, between [0, num_frames].
		 * @return A sample value, between [-1, 1].
		 *------------------------------------------------------------------------*/
		virtual sample get(double offset)
		{
			double frame = this->offset_to_frame(offset);
			return this->get_frame(frame);
		}

		/**------------------------------------------------------------------------
		 * Fill the buffer with a constant value.
		 *------------------------------------------------------------------------*/
		void fill(sample value)
		{
			for (int channel = 0; channel < this->num_channels; channel++)
			{
				for (int frame = 0; frame < this->num_frames; frame++)
				{
					this->data[channel][frame] = value;
				}
			}
		}

		/**------------------------------------------------------------------------
		 * Fill the buffer based on a transfer function
		 *------------------------------------------------------------------------*/
		void fill(transfer_fn f)
		{
			for (int channel = 0; channel < this->num_channels; channel++)
			{
				for (int frame = 0; frame < this->num_frames; frame++)
				{
					double offset = this->frame_to_offset(frame);
					this->data[channel][frame] = f(offset);
				}
			}
		}
	};

	/**-------------------------------------------------------------------------
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
				this->fill(1.0);
			}

			/**------------------------------------------------------------------------
			 * @param position An envelope position between [0, 1].
			 * @return An envelope amplitude value, between [0, 1].
			 *------------------------------------------------------------------------*/
			virtual double offset_to_frame(double offset) override
			{
				return map(offset, 0, 1, 0, this->num_frames - 1);
			}

			virtual double frame_to_offset(double frame) override
			{
				return map(frame, 0, this->num_frames - 1, 0, 1);
			}

			virtual void fill_exponential(float mu)
			{
				for (int x = 0; x < this->num_frames; x++)
					this->data[0][x] = random_exponential_pdf((float) x / this->num_frames, mu);
			}

			virtual void fill_beta(float a, float b)
			{
				for (int x = 0; x < this->num_frames; x++)
					this->data[0][x] = random_beta_pdf((float) x / this->num_frames, a, b);
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
				 * Initialise to a 1-to-1 linear mapping.
				 *-----------------------------------------------------------------------*/
				this->fill([](float input) { return input; });
			}

			/**------------------------------------------------------------------------
			 * Perform a waveshaper x -> f(x) transform.
			 *
			 * @param input A given input sample, between [-1, 1]
			 * @return A transformed sample value, between [-1, 1].
			 *------------------------------------------------------------------------*/
			virtual double offset_to_frame(double offset) override
			{
				return map(offset, -1, 1, 0, this->num_frames - 1);
			}

			virtual double frame_to_offset(double frame) override
			{
				return map(frame, 0, this->num_frames - 1, -1, 1);
			}
	};
}
