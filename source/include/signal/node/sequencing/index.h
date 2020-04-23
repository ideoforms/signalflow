#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
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
