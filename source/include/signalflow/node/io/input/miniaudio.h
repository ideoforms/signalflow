#pragma once

#define AudioIn AudioIn_MiniAudio

#include <soundio/soundio.h>
#include <vector>

#include "abstract.h"

#include "../output/miniaudio-library.h"
#include "signalflow/core/graph.h"

namespace signalflow
{

class AudioIn_MiniAudio : public AudioIn_Abstract
{
public:
    AudioIn_MiniAudio(unsigned int num_channels = 1);
    virtual ~AudioIn_MiniAudio() override;
    virtual int init() override;
    virtual int start() override;
    virtual int stop() override;
    virtual int destroy() override;
    virtual void process(Buffer &out, int num_samples) override;

private:
    std::string backend_name;
    std::string device_name;
    ma_context context;
    ma_device device;
    unsigned int num_channels;
};

}
