#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/asr.h"
#include <limits.h>

namespace signalflow
{

EnvelopeASR::EnvelopeASR(NodeRef attack, NodeRef sustain, NodeRef release, NodeRef clock)
    : attack(attack), sustain(sustain), release(release), clock(clock)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "envelope-asr";
    this->curve = SIGNALFLOW_CURVE_LINEAR;

    this->create_input("attack", this->attack);
    this->create_input("sustain", this->sustain);
    this->create_input("release", this->release);
    this->create_input("clock", this->clock);
    this->phase = std::vector<float>(this->num_output_channels, std::numeric_limits<float>::max());
}

void EnvelopeASR::trigger(std::string name, float value)
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

void EnvelopeASR::process(Buffer &out, int num_frames)
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

            float attack = this->attack->out[0][frame];
            float sustain = this->sustain->out[0][frame];
            float release = this->release->out[0][frame];

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

            if (this->curve == SIGNALFLOW_CURVE_EXPONENTIAL)
            {
                rv = signalflow_db_to_amplitude((rv - 1) * 60);
            }
            else if (this->curve == SIGNALFLOW_CURVE_LINEAR)
            {
            }
            else
            {
                throw std::runtime_error("Invalid curve value");
            }

            out[channel][frame] = rv;
        }
    }
}

}
