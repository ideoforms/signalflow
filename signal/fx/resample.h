#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class Resample : public UnaryOpNode
	{
		public:
			Resample(NodeRef input, NodeRef sample_rate, NodeRef bit_rate) :
				UnaryOpNode(input), sample_rate(sample_rate), bit_rate(bit_rate)
			{
				phase = 0.0;
				phase_last = -1.0;
				memset(sample_last, 0, sizeof(sample_last));

				this->name = "resample";
				this->add_param("sample_rate", this->sample_rate);
				this->add_param("bit_rate", this->bit_rate);
			}

			NodeRef sample_rate;
			NodeRef bit_rate;

			float phase;
			float phase_last;
			sample sample_last[SIGNAL_MAX_CHANNELS];

			virtual void next(sample **out, int num_frames);
	};
}
