#include "../constants.h"
#include "../node.h"

namespace libsignal
{

    class Counter : public Node
    {

    public:

        Counter(NodeRef clock = 0, NodeRef min = 0, NodeRef max = 0);

        virtual void process(sample **out, int num_frames);
        virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1.0);

        NodeRef clock;
        NodeRef min;
        NodeRef max;

        int counter[SIGNAL_MAX_CHANNELS];
    };

    REGISTER(Counter, "counter");

}
