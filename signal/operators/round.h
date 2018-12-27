#include "../constants.h"

#include "../node.h"
#include "../util.h"
#include "../oscillators/constant.h"

namespace libsignal
{

class RoundToScale : public UnaryOpNode
{

public:

    RoundToScale(NodeRef a);

    virtual void process(sample **out, int num_frames);

};

}
