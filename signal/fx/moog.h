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
				this->name = "eq";
				this->add_param("cutoff", this->cutoff);
				this->add_param("resonance", this->resonance);

				int int_buf_size = SIGNAL_MAX_CHANNELS * sizeof(int);
			}

			NodeRef cutoff;
			NodeRef resonance;

			float out1 = 0, out2 = 0, out3 = 0, out4 = 0;
			float in1 = 0, in2 = 0, in3 = 0, in4 = 0;

			virtual void next(sample **out, int num_frames);
	};

	REGISTER(MoogVCF, "moog");
}
