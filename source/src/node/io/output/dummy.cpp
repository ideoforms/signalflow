#include "signalflow/node/io/output/dummy.h"

namespace signalflow
{

AudioOut_Dummy::AudioOut_Dummy(unsigned int num_channels,
                               unsigned int sample_rate,
                               unsigned int buffer_size)
    : AudioOut_Abstract()
{
    this->name = "audioout-dummy";
    this->set_channels(num_channels, num_channels);
    this->sample_rate = sample_rate;
    this->buffer_size = buffer_size;
}

} // namespace signalflow
