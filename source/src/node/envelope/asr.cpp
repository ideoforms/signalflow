#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/asr.h"

namespace signalflow
{

EnvelopeASR::EnvelopeASR(NodeRef attack, NodeRef sustain, NodeRef release, NodeRef clock)
    : attack(attack), sustain(sustain), release(release), clock(clock)
{
    this->name = "envelope-asr";
    this->curve = SIGNAL_CURVE_EXPONENTIAL;

    this->create_input("attack", this->attack);
    this->create_input("sustain", this->sustain);
    this->create_input("release", this->release);
    this->create_input("clock", this->clock);
    this->phase = std::vector<float>(this->num_output_channels, 0.0);
}

void EnvelopeASR::trigger(std::string name, float value)
{
    if (name == SIGNAL_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            this->phase[channel] = 0.0;
        }
        this->state = SIGNAL_NODE_STATE_ACTIVE;
    }
}

void EnvelopeASR::process(sample **out, int num_frames)
{
    sample rv;

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool trigger = SIGNAL_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
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

                if (this->state == SIGNAL_NODE_STATE_ACTIVE)
                {
                    this->set_state(SIGNAL_NODE_STATE_FINISHED);
                }
            }

            this->phase[channel] += 1.0 / this->graph->get_sample_rate();

            if (this->curve == SIGNAL_CURVE_EXPONENTIAL)
            {
                rv = signal_db_to_amp((rv - 1) * 60);
            }
            else if (this->curve == SIGNAL_CURVE_LINEAR)
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
