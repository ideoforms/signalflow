#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class EQ : public UnaryOpNode
	{
		public:
			EQ(NodeRef input, NodeRef low_gain = 1.0, NodeRef mid_gain = 1.0, NodeRef high_gain = 1.0,
					NodeRef low_freq = 500, NodeRef high_freq = 5000) :
				UnaryOpNode(input), low_gain(low_gain), mid_gain(mid_gain), high_gain(high_gain),
					low_freq(low_freq), high_freq(high_freq)
			{
				this->name = "eq";
				this->add_param("low_gain", this->low_gain);
				this->add_param("mid_gain", this->mid_gain);
				this->add_param("high_gain", this->high_gain);
				this->add_param("low_freq", this->low_freq);
				this->add_param("high_freq", this->high_freq);
			}

			NodeRef low_gain;
			NodeRef mid_gain;
			NodeRef high_gain;
			NodeRef low_freq;
			NodeRef high_freq;

			float lf = 0, hf = 0;
			float f1p0 = 0, f1p1 = 0, f1p2 = 0, f1p3 = 0;
			float f2p0 = 0, f2p1 = 0, f2p2 = 0, f2p3 = 0;
			float sdm1 = 0, sdm2 = 0, sdm3 = 0;

			virtual void next(sample **out, int num_frames);
	};
}
