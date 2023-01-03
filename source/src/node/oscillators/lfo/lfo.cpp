#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/lfo.h"

namespace signalflow
{

LFO::LFO(NodeRef frequency, NodeRef min, NodeRef max, NodeRef phase)
    : frequency(frequency), min(min), max(max), phase(phase)
{
    SIGNALFLOW_CHECK_GRAPH();
    this->create_input("frequency", this->frequency);
    this->create_input("min", this->min);
    this->create_input("max", this->max);
    this->create_input("phase", this->phase);
    this->alloc();
}

void LFO::alloc()
{
    this->current_phase.resize(this->num_output_channels_allocated);
}

}
