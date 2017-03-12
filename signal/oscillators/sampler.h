#include "../node.h"
#include "../constants.h"
#include "../buffer.h"

namespace libsignal
{
	class Sampler : public Node
	{
		public:
			Sampler(BufferRef buffer = nullptr, NodeRef rate = 1.0, PropertyRef loop = false);

			BufferRef buffer;

			NodeRef rate;
			float phase;

			PropertyRef loop = 0;

			virtual void trigger(std::string = SIGNAL_DEFAULT_TRIGGER, float value = 0.0);
			virtual void process(sample **out, int num_frames);
	};

	REGISTER(Sampler, "sampler");
}
