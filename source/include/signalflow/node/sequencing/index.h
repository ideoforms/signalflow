#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Index : public Node
{

public:
    Index(PropertyRef list = {}, NodeRef index = 0);

    virtual void process(sample **out, int num_frames);

    PropertyRef list;
    NodeRef index;
};

REGISTER(Index, "index")

}
