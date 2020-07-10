#pragma once

#include "signalflow/core/platform.h"

#if __APPLE__

#if TARGET_OS_IPHONE

#define AudioOut AudioOut_iOS

#include "abstract.h"

#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class AudioOut_iOS : public AudioOut_Abstract
{
public:
    AudioOut_iOS(AudioGraph *graph);

    virtual int init() override;
    virtual int start() override;
    virtual int close() override;
};

} // namespace signalflow

#endif

#endif
