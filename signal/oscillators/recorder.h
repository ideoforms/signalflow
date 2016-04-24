#include "../node.h"
#include "../constants.h"
#include "../buffer.h"

namespace libsignal
{
	class Recorder : public Node
	{
		public:
			Recorder(Buffer *buffer = NULL, NodeRef input = 0.0, bool loop = false);

			Buffer *buffer;

			NodeRef input;

			float phase;
			bool loop;

			virtual void next(sample **out, int num_frames);
	};

	REGISTER(Recorder, "recorder");
}
