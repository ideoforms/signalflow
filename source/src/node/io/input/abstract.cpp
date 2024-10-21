#include "signalflow/node/io/input/abstract.h"

namespace signalflow
{

AudioIn_Abstract::AudioIn_Abstract()
{
    this->name = "audioin";
    this->set_channels(0, 1);
}

}
