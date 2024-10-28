#pragma once

#include <vector>

#include "abstract.h"

#include "../output/miniaudio-library.h"
#include "signalflow/core/graph.h"

namespace signalflow
{

class AudioIn : public AudioIn_Abstract
{
public:
    AudioIn(unsigned int num_channels = 1);
    virtual ~AudioIn() override;
    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;
    virtual void destroy() override;
    virtual void process(Buffer &out, int num_samples) override;

private:
    std::string backend_name;
    std::string device_name;
    ma_context context;
    ma_device device;
    unsigned int num_channels;
};

}
