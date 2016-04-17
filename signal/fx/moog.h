#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class MoogVCF : public UnaryOpNode
	{
		public:
			MoogVCF(NodeRef input = 0.0, NodeRef cutoff = 200.0, NodeRef resonance = 0.0) :
				UnaryOpNode(input), cutoff(cutoff), resonance(resonance)
			{
				this->name = "moog";
				this->add_param("cutoff", this->cutoff);
				this->add_param("resonance", this->resonance);

				int int_buf_size = SIGNAL_MAX_CHANNELS * sizeof(int);
				memset(out1, 0, int_buf_size);
				memset(out2, 0, int_buf_size);
				memset(out3, 0, int_buf_size);
				memset(out4, 0, int_buf_size);
				memset(in1, 0, int_buf_size);
				memset(in2, 0, int_buf_size);
				memset(in3, 0, int_buf_size);
				memset(in4, 0, int_buf_size);
			}

			NodeRef cutoff;
			NodeRef resonance;

			float out1[SIGNAL_MAX_CHANNELS],
				  out2[SIGNAL_MAX_CHANNELS],
				  out3[SIGNAL_MAX_CHANNELS],
				  out4[SIGNAL_MAX_CHANNELS];
			float in1[SIGNAL_MAX_CHANNELS],
				  in2[SIGNAL_MAX_CHANNELS],
				  in3[SIGNAL_MAX_CHANNELS],
				  in4[SIGNAL_MAX_CHANNELS];

			virtual void next(sample **out, int num_frames);
	};

	REGISTER(MoogVCF, "moog");
}
