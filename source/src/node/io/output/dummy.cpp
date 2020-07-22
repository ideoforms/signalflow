#include "signalflow/node/io/output/dummy.h"

namespace signalflow
{

AudioOut_Dummy::AudioOut_Dummy(int num_channels)
    : AudioOut_Abstract()
{
    this->name = "audioout-dummy";
    this->set_channels(num_channels, 0);
    this->sample_rate = 44100;
}

} // namespace signalflow
