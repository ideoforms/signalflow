#pragma once

#ifdef HAVE_SOUNDIO

#define AudioOut AudioOut_SoundIO

#include <soundio/soundio.h>
#include <vector>

#include "abstract.h"

#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class AudioOut_SoundIO : public AudioOut_Abstract
{
public:
    AudioOut_SoundIO(AudioGraph *graph);
    virtual int init() override;
    virtual int start() override;
    virtual int stop() override;
    virtual int destroy() override;

    struct SoundIo *soundio;
    struct SoundIoDevice *device;
    struct SoundIoOutStream *outstream;
};

} // namespace signalflow

#endif
