#pragma once

#ifdef HAVE_SOUNDIO

#define AudioIn AudioIn_SoundIO

#include <soundio/soundio.h>
#include <vector>

#include "abstract.h"

#include "signalflow/core/graph.h"

namespace signalflow
{
class AudioIn_SoundIO : public AudioIn_Abstract
{
public:
    AudioIn_SoundIO();
    virtual int init() override;
    virtual int start() override;
    virtual int stop() override;
    virtual int destroy() override;
    virtual void process(sample **out, int num_samples) override;

    struct SoundIo *soundio;
    struct SoundIoDevice *device;
    struct SoundIoInStream *instream;

    Buffer *buffer;
    int read_pos;
    int write_pos;
};
}

#endif
