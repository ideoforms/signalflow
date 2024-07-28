#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Trigger segments of `buffer` at the given list of `onsets` positions, in
 * seconds. `index` determines the index of the onset to play back at, which can
 * also be passed as an argument to trigger(). `rate` determines the playback rate,
 * and `clock` can be used to retrigger based on the output of another Node.
 * If `continue_after_segment` is non-zero, playback will continue after the
 * subsequent onset.
 *---------------------------------------------------------------------------------*/
class SegmentPlayer : public Node
{
public:
    SegmentPlayer(BufferRef buffer = nullptr,
                  std::vector<float> onsets = {},
                  NodeRef index = nullptr,
                  NodeRef rate = 1.0,
                  NodeRef clock = nullptr,
                  NodeRef continue_after_segment = 0);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER,
                         float value = SIGNALFLOW_NULL_FLOAT) override;
    virtual void set_buffer(std::string, BufferRef buffer) override;

protected:
    BufferRef buffer;
    PropertyRef onsets;

    /*--------------------------------------------------------------------------------
     * If phase is stored as a float, rounding errors quickly accumulate for
     * non-integer rates and cause detuning. Should investigate whether this also
     * affects other oscillators...
     *--------------------------------------------------------------------------------*/
    double phase;
    double segment_end_phase;
    NodeRef index;
    NodeRef rate;
    float rate_scale_factor;
    NodeRef clock;
    NodeRef continue_after_segment;
};

REGISTER(SegmentPlayer, "segment-player")
}
