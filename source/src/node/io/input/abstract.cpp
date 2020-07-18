#include "signalflow/node/io/input/abstract.h"

namespace signalflow
{
AudioIn_Abstract *shared_in = NULL;

AudioIn_Abstract::AudioIn_Abstract()
{
    if (shared_in)
        throw std::runtime_error("Multiple AudioIn nodes are not yet supported.");

    shared_in = this;

    this->name = "audioin";
    this->num_output_channels = 1;
    this->num_input_channels = 0;
    this->matches_input_channels = false;
}

}
