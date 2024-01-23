#include "grain.h"
#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Segmented Granulator.
 *---------------------------------------------------------------------------------*/
class SegmentedGranulator : public Node
{
public:
    SegmentedGranulator(BufferRef buffer = nullptr,
                        std::vector<float> onset_times = {},
                        std::vector<float> durations = {},
                        NodeRef index = 0.0,
                        NodeRef rate = 1.0,
                        NodeRef clock = 0,
                        NodeRef max_grains = 2048);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void set_buffer(std::string, BufferRef buffer) override;

protected:
    BufferRef buffer;
    BufferRef envelope;

    NodeRef index;
    NodeRef rate;
    NodeRef clock;
    NodeRef max_grains;

    std::vector<float> onset_times;
    std::vector<float> durations;
    float rate_scale_factor;
    std::vector<Grain *> grains;
};

REGISTER(SegmentedGranulator, "segmented-granulator")
}
