#pragma once

#ifdef HAVE_SOUNDIO

#define AudioOut AudioOut_SoundIO

#include <soundio/soundio.h>
#include <vector>

#include "abstract.h"

#include "../../unit.h"
#include "../../buffer.h"
#include "../../graph.h"

namespace signum
{

    class AudioOut_SoundIO : public AudioOut_Abstract
    {
    public:
        AudioOut_SoundIO(Graph *graph);
        virtual int init() override;
        virtual int start() override;
        virtual int close() override;

        struct SoundIo *soundio;
        struct SoundIoDevice *device;
        struct SoundIoOutStream *outstream;
    };

} // namespace signum

#endif
