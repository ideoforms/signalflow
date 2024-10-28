#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/io/output/abstract.h"

namespace signalflow
{

class AudioOut_Dummy : public AudioOut_Abstract
{
public:
    AudioOut_Dummy(int num_channels = 2, int buffer_size = 256);

    virtual void init() {}
    virtual void start() {}
    virtual void stop() {}
    virtual void destroy() {}
};

REGISTER(AudioOut_Dummy, "audioout-dummy")

} // namespace signalflow
