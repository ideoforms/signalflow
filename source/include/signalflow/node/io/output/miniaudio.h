#pragma once

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

    static std::list<std::string> get_output_device_names(std::string backend_name = "");
    static std::list<std::string> get_input_device_names(std::string backend_name = "");
    static std::list<std::string> get_backend_names();

    /*--------------------------------------------------------------------------------
     * Initialise a new miniaudio context, using the specified backend name if
     * present, or the default backend otherwise.
     *
     * Public because AudioIn also uses this method.
     *-------------------------------------------------------------------------------*/
    static void init_context(ma_context *context, std::string backend_name = "");

private:
    std::string backend_name;
    std::string device_name;
    ma_context context;
    ma_device device;
};

REGISTER(AudioOut, "audioout")

} // namespace signalflow
