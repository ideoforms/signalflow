#pragma once 

#include "../node.h"

namespace libsignal
{
	class Noise : public Node
	{
	public:
		Noise(NodeRef frequency = 0.0, bool interpolate = true, NodeRef min = 0.0, NodeRef max = 1.0);

		NodeRef frequency;
		bool interpolate;

		NodeRef min;
		NodeRef max;

		sample value[SIGNAL_MAX_CHANNELS];

		virtual void next(sample **out, int num_frames);
	
	private:

		int steps_remaining[SIGNAL_MAX_CHANNELS];
		float step_change[SIGNAL_MAX_CHANNELS];

	};

	REGISTER(Noise, "noise");
}
