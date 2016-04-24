#pragma once

#include "../constants.h"
#include "../node.h"

namespace libsignal
{

class ASR : public Node
{
	public:
		ASR(NodeRef attack = 0.1, NodeRef sustain = 0.5, NodeRef release = 0.1, NodeRef clock = nullptr);

		NodeRef attack;
		NodeRef sustain;
		NodeRef release;
		NodeRef clock = nullptr;

		float phase;
		float clock_last;


		virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1.0);
		virtual void process(sample **out, int num_frames);
};

REGISTER(ASR, "env-asr");

}
