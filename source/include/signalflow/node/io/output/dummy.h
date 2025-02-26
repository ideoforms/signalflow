#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/io/output/abstract.h"

namespace signalflow
{

class AudioOut_Dummy : public AudioOut_Abstract
{
public:
    AudioOut_Dummy(unsigned int num_channels = 2,
                   unsigned int sample_rate = SIGNALFLOW_DEFAULT_SAMPLE_RATE,
                   unsigned int buffer_size = SIGNALFLOW_DEFAULT_BLOCK_SIZE);

    virtual void init() {}
    virtual void start() {}
    virtual void stop() {}
    virtual void destroy() {}
};

REGISTER(AudioOut_Dummy, "audioout-dummy")

} // namespace signalflow
