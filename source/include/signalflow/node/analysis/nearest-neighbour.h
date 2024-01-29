#include "signalflow/core/kdtree.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Nearest Neighbour.
 *---------------------------------------------------------------------------------*/
class NearestNeighbour : public Node
{
public:
    NearestNeighbour(BufferRef buffer = nullptr, NodeRef target = 0.0);

    virtual void set_buffer(std::string name, BufferRef buffer);
    virtual void process(Buffer &out, int num_frames);

private:
    BufferRef buffer;
    NodeRef target;
    KDTree *kdtree;
};

REGISTER(NearestNeighbour, "nearest-neighbour")
}
