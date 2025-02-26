#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Increments by one for each audio block processed.
 *---------------------------------------------------------------------------------*/
class BlockCounter : public Node
{

public:
    BlockCounter();

    virtual void process(Buffer &out, int num_frames) override;

private:
    unsigned int counter;
};

REGISTER(BlockCounter, "block-counter")

}
