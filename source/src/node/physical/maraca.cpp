#include "signalflow/node/physical/maraca.h"

#include "signalflow/buffer/envelope-buffer.h"
#include "signalflow/core/graph.h"

#include <math.h>

namespace signalflow
{

/*--------------------------------------------------------------------------------
 *
 *--------------------------------------------------------------------------------*/

Maraca::Maraca(NodeRef num_beans,
               NodeRef shake_decay,
               NodeRef grain_decay,
               NodeRef shake_duration,
               NodeRef shell_frequency,
               NodeRef shell_resonance,
               NodeRef clock,
               NodeRef energy)
    : StochasticNode(), num_beans(num_beans), shake_decay(shake_decay), grain_decay(grain_decay), shake_duration(shake_duration), shell_frequency(shell_frequency), shell_resonance(shell_resonance), clock(clock), energy(energy)
{
    this->name = "maraca";

    this->create_input("num_beans", this->num_beans);
    this->create_input("shake_decay", this->shake_decay);
    this->create_input("grain_decay", this->grain_decay);
    this->create_input("shake_duration", this->shake_duration);
    this->create_input("shell_frequency", this->shell_frequency);
    this->create_input("shell_resonance", this->shell_resonance);
    this->create_input("clock", this->clock);
    this->create_input("energy", this->energy);

    this->envelope = new EnvelopeBuffer("hanning", graph->get_sample_rate());
    this->create_buffer("envelope", this->envelope);

    this->alloc();
}

void Maraca::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        this->phase = 0.0;
        if (value != SIGNALFLOW_NULL_FLOAT)
        {
            this->shake_intensity = value;
        }
        else
        {
            this->shake_intensity = 1.0;
        }
    }
    else
    {
        this->Node::trigger(name, value);
    }
}

void Maraca::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float shell_resonance = this->shell_resonance->out[0][frame];
        float shell_frequency = this->shell_frequency->out[0][frame];

        coefs[0] = -shell_resonance * 2.0 * cos(shell_frequency * M_PI * 2 / this->graph->get_sample_rate());
        coefs[1] = shell_resonance * shell_resonance;

        SIGNALFLOW_PROCESS_TRIGGER(this->clock, frame, SIGNALFLOW_DEFAULT_TRIGGER);

        if (this->phase > -1)
        {
            this->shake_energy += this->envelope->get_frame(0, this->phase) * this->shake_intensity;
            float duration = this->shake_duration->out[0][frame];
            float phase_increment = this->envelope->get_duration() / duration;
            this->phase += phase_increment;
            if (this->phase >= this->envelope->get_num_frames())
            {
                this->phase = -1;
            }
        }

        int num_beans = this->num_beans->out[0][frame];
        if (num_beans < 1)
        {
            num_beans = 1;
        }

        /*--------------------------------------------------------------------------------
         * Energy injection.
         *--------------------------------------------------------------------------------*/
        if (this->energy)
        {
            this->shake_energy += this->energy->out[0][frame];
        }

        /*--------------------------------------------------------------------------------
         * Energy decay, exponential.
         *--------------------------------------------------------------------------------*/
        this->shake_energy *= this->shake_decay->out[0][frame];

        if (random_uniform(0, 1024) < num_beans)
        {
            /*--------------------------------------------------------------------------------
             * A grain collision has occurred.
             *--------------------------------------------------------------------------------*/
            float gain = log(num_beans) / log(4.0) * 0.0025 / num_beans;
            this->sound_level += gain * this->shake_energy;
        }

        float input = this->sound_level * random_uniform(-1.0, 1.0);
        this->sound_level *= this->grain_decay->out[0][frame];

        input -= output[0] * coefs[0];
        input -= output[1] * coefs[1];
        output[1] = output[0];
        output[0] = input;
        float rv = output[0] - output[1];

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = rv;
        }
    }
}

}
