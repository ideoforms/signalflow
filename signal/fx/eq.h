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

				int int_buf_size = SIGNAL_MAX_CHANNELS * sizeof(int);
				memset(f1p0, 0, int_buf_size);
				memset(f1p1, 0, int_buf_size);
				memset(f1p2, 0, int_buf_size);
				memset(f1p3, 0, int_buf_size);
				memset(f2p0, 0, int_buf_size);
				memset(f2p1, 0, int_buf_size);
				memset(f2p2, 0, int_buf_size);
				memset(f2p3, 0, int_buf_size);
				memset(sdm1, 0, int_buf_size);
				memset(sdm2, 0, int_buf_size);
				memset(sdm3, 0, int_buf_size);
			}

			NodeRef low_gain;
			NodeRef mid_gain;
			NodeRef high_gain;
			NodeRef low_freq;
			NodeRef high_freq;

			float f1p0[SIGNAL_MAX_CHANNELS], f1p1[SIGNAL_MAX_CHANNELS], f1p2[SIGNAL_MAX_CHANNELS], f1p3[SIGNAL_MAX_CHANNELS];
			float f2p0[SIGNAL_MAX_CHANNELS], f2p1[SIGNAL_MAX_CHANNELS], f2p2[SIGNAL_MAX_CHANNELS], f2p3[SIGNAL_MAX_CHANNELS];
			float sdm1[SIGNAL_MAX_CHANNELS], sdm2[SIGNAL_MAX_CHANNELS], sdm3[SIGNAL_MAX_CHANNELS];

			virtual void next(sample **out, int num_frames);
	};
}
