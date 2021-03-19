#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/adsr.h"

namespace signalflow
{

EnvelopeADSR::EnvelopeADSR(NodeRef attack, NodeRef decay, NodeRef sustain, NodeRef release, NodeRef gate)
    : attack(attack), decay(decay), sustain(sustain), release(release), gate(gate)
{
    this->phase = 0.0;

    this->name = "envelope-adsr";
    this->create_input("attack", this->attack);
    this->create_input("decay", this->decay);
    this->create_input("sustain", this->sustain);
    this->create_input("release", this->release);
    this->create_input("gate", this->gate);

    this->curve = SIGNALFLOW_CURVE_LINEAR;
    this->released = false;
    this->level = 0.0;
}

void EnvelopeADSR::process(Buffer &out, int num_frames)
{
    float phase_step = 1.0f / this->graph->get_sample_rate();

    for (int frame = 0; frame < num_frames; frame++)
    {
        if (SIGNALFLOW_CHECK_TRIGGER(gate, frame))
        {
            this->phase = 0.0;
            this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
            this->released = false;
        }
        float attack = this->attack->out[0][frame];
        float decay = this->decay->out[0][frame];
        float sustain = this->sustain->out[0][frame];
        float release = this->release->out[0][frame];
        float gate = this->gate->out[0][frame];
        if (gate == 0.0 && !this->released)
        {
            this->released = true;
            if (release > 0)
            {
                this->release_decrement_per_frame = this->level / (release * graph->get_sample_rate());
            }
            else
            {
                this->release_decrement_per_frame = this->level;
            }
        }

        if (this->released)
        {
            /*------------------------------------------------------------------------
             * Release phase.
             *-----------------------------------------------------------------------*/
            this->level -= this->release_decrement_per_frame;
            if (this->level <= 0)
            {
                /*------------------------------------------------------------------------
                 * Envelope has finished.
                 *-----------------------------------------------------------------------*/
                this->level = 0.0;

                if (this->state == SIGNALFLOW_NODE_STATE_ACTIVE)
                {
                    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
                }
            }
        }
        else if (this->phase < attack)
        {
            /*------------------------------------------------------------------------
             * Attack phase.
             *-----------------------------------------------------------------------*/
            this->level = (this->phase / attack);
        }
        else if (this->phase <= attack + decay)
        {
            /*------------------------------------------------------------------------
             * Sustain phase.
             *-----------------------------------------------------------------------*/
            float proportion_through_decay = ((this->phase - attack) / decay);
            this->level = sustain + (1.0 - proportion_through_decay) * (1.0 - sustain);
        }
        else
        {
            this->level = sustain;
        }
        this->phase += phase_step;

        float rv;
        if (this->curve == SIGNALFLOW_CURVE_EXPONENTIAL)
        {
            if (this->level > 0)
            {
                rv = signalflow_db_to_amplitude((this->level - 1) * 60);
            }
        }
        else if (this->curve == SIGNALFLOW_CURVE_LINEAR)
        {
            // no adjustment needed
            rv = this->level;
        }
        else
        {
            throw std::runtime_error("Invalid curve value");
        }

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = rv;
        }
    }
}

}
