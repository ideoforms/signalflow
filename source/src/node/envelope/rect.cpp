#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/rect.h"
#include <limits>
#include <math.h>

namespace signalflow
{

EnvelopeRect::EnvelopeRect(NodeRef sustain, NodeRef clock)
    : sustain(sustain), clock(clock)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "envelope-rect";

    this->create_input("sustain", this->sustain);
    this->create_input("clock", this->clock);
    this->phase = std::vector<float>(this->num_output_channels, std::numeric_limits<float>::max());

    if (clock == nullptr)
    {
        this->trigger();
    }
}

void EnvelopeRect::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            this->phase[channel] = 0.0;
        }
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
}

void EnvelopeRect::process(Buffer &out, int num_frames)
{
    sample rv;
    float phase_change = 1.0 / this->graph->get_sample_rate();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame))
            {
                this->phase[channel] = 0.0;
                this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
            }

            float sustain = this->sustain->out[0][frame];

            if (this->phase[channel] < sustain)
            {
                /*------------------------------------------------------------------------
                 * Sustain phase.
                 *-----------------------------------------------------------------------*/
                rv = 1.0;
            }
            else
            {
                /*------------------------------------------------------------------------
                 * Envelope has finished.
                 *-----------------------------------------------------------------------*/
                rv = 0.0;

                if (this->state == SIGNALFLOW_NODE_STATE_ACTIVE)
                {
                    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
                }
            }

            this->phase[channel] += phase_change;
            out[channel][frame] = rv;
        }
    }
}

}
