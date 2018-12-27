#include "../node.h"
#include "../constants.h"
#include "../buffer.h"

namespace libsignal
{
    class Recorder : public Node
    {
        public:
            Recorder(BufferRef buffer = nullptr, NodeRef input = 0.0, bool loop = false);

            BufferRef buffer;

            NodeRef input;

            float phase;
            bool loop;

            virtual void process(sample **out, int num_frames);
    };

    REGISTER(Recorder, "recorder");
}
