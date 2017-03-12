#include "../node.h"
#include "../constants.h"
#include "../buffer.h"

#define SIGNAL_SAMPLER_TRIGGER_SET_POSITION      "position"
#define SIGNAL_SAMPLER_TRIGGER_SET_LENGTH        "length"
#define SIGNAL_SAMPLER_TRIGGER_SET_RECORD        "record"
#define SIGNAL_SAMPLER_TRIGGER_SET_LOOP_RECORD   "loop_record"
#define SIGNAL_SAMPLER_TRIGGER_SET_LOOP_PLAYBACK "loop"

namespace libsignal
{
	class Sampler : public Node
	{
		public:
			Sampler(BufferRef buffer = nullptr, NodeRef rate = 1.0, PropertyRef loop = 0);

			BufferRef buffer;

			NodeRef input = nullptr;
			NodeRef rate = nullptr;
			NodeRef loop_start = nullptr;
			NodeRef loop_end = nullptr;

			PropertyRef loop = 0;
			PropertyRef loop_record = 0;

			virtual void trigger(std::string = SIGNAL_SAMPLER_TRIGGER_SET_POSITION, float value = 0.0);
			virtual void process(sample **out, int num_frames);

		private:
			float phase;
	};

	REGISTER(Sampler, "sampler");
}
