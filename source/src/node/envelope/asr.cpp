#include "signal/node/envelope/asr.h"
#include "signal/core/graph.h"

namespace libsignal
{

EnvelopeASR::EnvelopeASR(NodeRef attack, NodeRef sustain, NodeRef release, NodeRef clock)
    : attack(attack), sustain(sustain), release(release), clock(clock)
{
    this->phase = 0.0;

    this->name = "envelope-asr";
    this->add_input("attack", this->attack);
    this->add_input("sustain", this->sustain);
    this->add_input("release", this->release);
    this->add_input("clock", this->clock);
}

void EnvelopeASR::trigger(std::string name, float value)
{
    if (name == SIGNAL_DEFAULT_TRIGGER)
    {
        this->phase = 0.0;
        this->state = SIGNAL_NODE_STATE_ACTIVE;
    }
}

void EnvelopeASR::process(sample **out, int num_frames)
{
    sample rv;

    for (int frame = 0; frame < num_frames; frame++)
    {
        SIGNAL_PROCESS_TRIGGER(this->clock, frame, SIGNAL_DEFAULT_TRIGGER);

        float attack = this->attack->out[0][frame];
        float sustain = this->sustain->out[0][frame];
        float release = this->release->out[0][frame];

        if (this->phase < attack)
        {
            /*------------------------------------------------------------------------
             * Attack phase.
             *-----------------------------------------------------------------------*/
            rv = (this->phase / attack);
        }
        else if (this->phase <= attack + sustain)
        {
            /*------------------------------------------------------------------------
             * Sustain phase.
             *-----------------------------------------------------------------------*/
            rv = 1.0;
        }
        else if (this->phase < attack + sustain + release)
        {
            /*------------------------------------------------------------------------
             * Release phase.
             *-----------------------------------------------------------------------*/
            rv = 1.0 - (this->phase - (attack + sustain)) / release;
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

        this->phase += 1.0 / this->graph->sample_rate;

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = rv;
        }
    }
}

}
