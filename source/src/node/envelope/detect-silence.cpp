#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/detect-silence.h"
#include <limits>
#include <math.h>

namespace signalflow
{

DetectSilence::DetectSilence(NodeRef input, NodeRef threshold)
    : UnaryOpNode(input), threshold(threshold)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "detect-silence";

    this->create_input("threshold", this->threshold);
}

void DetectSilence::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        bool silent = true;
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (fabs(this->input->out[channel][frame]) > this->threshold->out[channel][frame])
            {
                silent = false;
            }
            out[channel][frame] = this->input->out[channel][frame];
        }
        if (silent && this->state == SIGNALFLOW_NODE_STATE_ACTIVE)
        {
            this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
        }
    }
}

}
