#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Euclidean : public Node
{

public:
    Euclidean(NodeRef clock = 0, NodeRef sequence_length = 0, NodeRef num_events = 0);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

private:
    NodeRef clock;
    NodeRef sequence_length;
    NodeRef num_events;
    int sequence_length_cur;
    int num_events_cur;

    void _recalculate(int sequence_length, int num_events);
    std::vector<int> events;
    int position;
};

REGISTER(Euclidean, "euclidean")

}
