#include "signalflow/node/node.h"

namespace signalflow
{
class OnsetDetector : public UnaryOpNode
{
public:
    OnsetDetector(NodeRef input = 0.0, NodeRef threshold = 2.0, NodeRef min_interval = 0.1);

    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef threshold;
    NodeRef min_interval;

    float fast_lag;
    float slow_lag;
    float fast_value;
    float slow_value;
    unsigned int interval_timer;
};

REGISTER(OnsetDetector, "onset-detector")
}
