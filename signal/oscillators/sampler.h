#include "../node.h"
#include "../constants.h"
#include "../buffer.h"

namespace libsignal
{
	class Sampler : public Node
	{
		public:
			Sampler(Buffer *buffer = NULL, NodeRef rate = 1.0, bool loop = false);

			Buffer *buffer;

			NodeRef rate;
			float phase;
			bool loop;

			virtual void trigger();
			virtual void next(sample **out, int num_frames);
	};

	REGISTER(Sampler, "sampler");
}
