#pragma once

#include <soundio/soundio.h>
#include <vector>

#include "abstract.h"

#include "miniaudio-library.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class AudioOut : public AudioOut_Abstract
{
public:
    AudioOut(const std::string &backend_name = "",
             const std::string &device_name = "",
             unsigned int sample_rate = 0,
             unsigned int buffer_size = 0);

    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;
    virtual void destroy() override;

    std::list<std::string> get_output_device_names();
    std::list<std::string> get_output_backend_names();
    int get_default_output_device_index();

private:
    /*--------------------------------------------------------------------------------
     * Initialise a new miniaudio context, using the specified backend name if
     * present, or the default backend otherwise.
     *-------------------------------------------------------------------------------*/
    void init_context(ma_context *context);

    std::string backend_name;
    std::string device_name;
    ma_context context;
    ma_device device;
};

REGISTER(AudioOut, "audioout")

} // namespace signalflow
