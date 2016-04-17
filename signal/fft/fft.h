#pragma once

#include "fftnode.h"

#include <Accelerate/Accelerate.h>

namespace libsignal
{
	class FFT : public FFTNode
	{
		public:
			FFT(NodeRef input = 0.0, int fft_size = SIGNAL_DEFAULT_FFT_SIZE);
			~FFT();

			virtual void fft(sample *in, sample *out, bool polar = true, bool do_window = true);
			virtual void next(sample **out, int num_frames);

			NodeRef input;
			int hop_size;
			int log2N;
			FFTSetup fft_setup;
			sample *buffer;
			sample *buffer2;
			sample *inbuf;
			int inbuf_size;
			sample *window;
	};

	REGISTER(FFT, "fft");
}

