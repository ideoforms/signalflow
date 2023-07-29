#include "signalflow/bus/bus.h"

namespace signalflow
{

Bus::Bus(int num_channels)
{
    this->num_channels = num_channels;
}

int Bus::get_num_channels()
{
    return this->num_channels;
}

}
