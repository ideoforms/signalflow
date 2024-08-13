#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/asr.h"
#include <limits>
#include <math.h>

namespace signalflow
{

ASREnvelope::ASREnvelope(NodeRef attack, NodeRef sustain, NodeRef release, NodeRef curve, NodeRef clock)
    : attack(attack), sustain(sustain), release(release), curve(curve), clock(clock)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "asr-envelope";

    this->create_input("attack", this->attack);
    this->create_input("sustain", this->sustain);
    this->create_input("release", this->release);
    this->create_input("curve", this->curve);
    this->create_input("clock", this->clock);
    this->phase = std::vector<float>(this->num_output_channels, std::numeric_limits<float>::max());

    if (!clock)
    {
        this->trigger();
    }
}

void ASREnvelope::trigger(std::string name, float value)
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

void ASREnvelope::process(Buffer &out, int num_frames)
{
    sample rv;

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool trigger = SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
            if (trigger)
            {
                this->phase[channel] = 0.0;
            }

            float attack = this->attack->out[channel][frame];
            float sustain = this->sustain->out[channel][frame];
            float release = this->release->out[channel][frame];

            if (this->phase[channel] < attack)
            {
                /*------------------------------------------------------------------------
                 * Attack phase.
                 *-----------------------------------------------------------------------*/
                rv = (this->phase[channel] / attack);
            }
            else if (this->phase[channel] <= attack + sustain)
            {
                /*------------------------------------------------------------------------
                 * Sustain phase.
                 *-----------------------------------------------------------------------*/
                rv = 1.0;
            }
            else if (this->phase[channel] < attack + sustain + release)
            {
                /*------------------------------------------------------------------------
                 * Release phase.
                 *-----------------------------------------------------------------------*/
                rv = 1.0 - (this->phase[channel] - (attack + sustain)) / release;
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

            this->phase[channel] += 1.0 / this->graph->get_sample_rate();

            if (this->curve->out[channel][frame] != 1.0)
            {
                rv = powf(rv, this->curve->out[channel][frame]);
            }

            out[channel][frame] = rv;
        }
    }
}

}
