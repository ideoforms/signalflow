#include "signalflow/node/io/output/dummy.h"

namespace signalflow
{

AudioOut_Dummy::AudioOut_Dummy(int num_channels, int buffer_size)
    : AudioOut_Abstract()
{
    this->name = "audioout-dummy";
    this->set_channels(num_channels, 0);
    this->sample_rate = SIGNALFLOW_DEFAULT_SAMPLE_RATE;
    this->buffer_size = buffer_size;
}

} // namespace signalflow
