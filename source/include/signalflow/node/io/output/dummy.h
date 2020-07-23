#pragma once

#include "signalflow/node/io/output/abstract.h"

namespace signalflow
{

class AudioOut_Dummy : public AudioOut_Abstract
{
public:
    AudioOut_Dummy(int num_channels = 2);

    virtual int init() { return 0; }
    virtual int start() { return 0; }
    virtual int stop() { return 0; }
    virtual int destroy() { return 0; }
};

REGISTER(AudioOut_Dummy, "audioout-dummy");

} // namespace signalflow
