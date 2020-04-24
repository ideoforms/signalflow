#include "signal/node/envelope/adsr.h"
#include "signal/core/graph.h"

namespace libsignal
{

ADSR::ADSR(NodeRef attack, NodeRef decay, NodeRef sustain, NodeRef release, NodeRef gate)
    : attack(attack), decay(decay), sustain(sustain), release(release), gate(gate)
{
    this->phase = 0.0;

    this->name = "env-adsr";
    this->add_input("attack", this->attack);
    this->add_input("decay", this->decay);
    this->add_input("sustain", this->sustain);
    this->add_input("release", this->release);
    this->add_input("gate", this->gate);
}

void ADSR::process(sample **out, int num_frames)
{
    sample rv;
    float phase_step = 1.0f / this->graph->sample_rate;

    for (int frame = 0; frame < num_frames; frame++)
    {
        float attack = this->attack->out[0][frame];
        float decay = this->decay->out[0][frame];
        float sustain = this->sustain->out[0][frame];
        float release = this->release->out[0][frame];
        float gate = this->gate->out[0][frame];
        if (gate == 0.0 && !this->released)
        {
            this->released = true;
        }

        if (this->phase < attack)
        {
            /*------------------------------------------------------------------------
             * Attack phase.
             *-----------------------------------------------------------------------*/
            rv = (this->phase / attack);
            this->phase += phase_step;
        }
        else if (this->phase <= attack + decay)
        {
            /*------------------------------------------------------------------------
             * Sustain phase.
             *-----------------------------------------------------------------------*/
            float proportion_through_decay = ((this->phase - attack) / decay);
            rv = sustain + (1.0 - proportion_through_decay) * (1.0 - sustain);
            this->phase += phase_step;
        }
        else
        {
            if (!released)
            {
                rv = sustain;
            }
            else
            {
                if (this->phase < attack + decay + release)
                {
                    /*------------------------------------------------------------------------
                     * Release phase.
                     *-----------------------------------------------------------------------*/
                    rv = sustain - sustain * (this->phase - (attack + decay)) / release;
                }
                else
                {
                    /*------------------------------------------------------------------------
                     * Envelope has finished.
                     *-----------------------------------------------------------------------*/
                    rv = 0.0;
                }
                this->phase += phase_step;
            }
        }

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = rv;
        }
    }
}

}
