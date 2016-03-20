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
		int phase;
		sample value;

		NodeRef min;
		NodeRef max;

		virtual void next(sample **out, int num_frames);
	
	private:
		int steps_remaining;
		float step_change;

	};

	REGISTER(Noise, "noise");
}
