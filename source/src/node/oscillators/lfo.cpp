#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/lfo.h"

namespace signalflow
{

LFO::LFO(NodeRef frequency, NodeRef min, NodeRef max)
    : frequency(frequency), min(min), max(max)
{
    SIGNALFLOW_CHECK_GRAPH();
    this->create_input("frequency", this->frequency);
    this->create_input("min", this->min);
    this->create_input("max", this->max);
    this->alloc();
}

void LFO::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

}
