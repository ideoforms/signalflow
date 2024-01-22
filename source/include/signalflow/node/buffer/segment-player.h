#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Trigger segments of a buffer at the given onset positions.
 *---------------------------------------------------------------------------------*/
class SegmentPlayer : public Node
{
public:
    SegmentPlayer(BufferRef buffer = nullptr,
                  std::vector<float> onsets = {},
                  NodeRef index = nullptr,
                  NodeRef rate = 1.0,
                  NodeRef clock = nullptr);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void set_buffer(std::string, BufferRef buffer) override;

protected:
    BufferRef buffer;
    PropertyRef onsets;

    /*
     * If phase is stored as a float, rounding errors quickly accumulate for non-integer rates
     * and cause detuning. Should investigate whether this also affects other oscillators...
     */
    double phase;
    NodeRef index;
    NodeRef rate;
    float rate_scale_factor;
    NodeRef clock;
};

REGISTER(SegmentPlayer, "segment-player")
}
