#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Applies wave-shaping as described in the WaveShaperBuffer `buffer`.
 *---------------------------------------------------------------------------------*/
class WaveShaper : public UnaryOpNode
{
public:
    WaveShaper(NodeRef input = 0.0, BufferRef buffer = nullptr);

    virtual void process(Buffer &out, int num_frames) override;

private:
    BufferRef buffer;
};

REGISTER(WaveShaper, "waveshaper")
}
