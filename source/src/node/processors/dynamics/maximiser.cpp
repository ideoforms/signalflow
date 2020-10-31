#include "signalflow/core/graph.h"
#include "signalflow/node/processors/dynamics/maximiser.h"

namespace signalflow
{

Maximiser::Maximiser(NodeRef input, NodeRef ceiling, NodeRef attack_time, NodeRef release_time)
    : UnaryOpNode(input), ceiling(ceiling), attack_time(attack_time), release_time(release_time)
{
    this->name = "maximiser";
    this->gain = 1.0;

    this->create_input("ceiling", this->ceiling);
    this->create_input("attack_time", this->attack_time);
    this->create_input("release_time", this->release_time);
}

void Maximiser::process(Buffer &out, int num_frames)
{
    float max_sample = 0.0;
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float value = fabsf(input->out[channel][frame]);
            if (value > max_sample)
            {
                max_sample = value;
            }
        }
    }

    float max_sample_after_gain = max_sample * this->gain;

    float ceiling = this->ceiling->out[0][0];
    float attack_time = this->attack_time->out[0][0];
    float release_time = this->release_time->out[0][0];
    float gain_change_per_sample = 0.0;

    if (max_sample_after_gain < ceiling)
    {
        /*--------------------------------------------------------------------------------*
         * Need to increase gain. Use attack_time to calculate gain change.
         **-------------------------------------------------------------------------------*/
        float attack_time_samples = attack_time * this->graph->get_sample_rate();
        if (max_sample_after_gain > 0.0)
        {
            float gain_change = ceiling / max_sample_after_gain;
            gain_change_per_sample = powf(gain_change, 1.0 / attack_time_samples);
        }
        else
        {
            gain_change_per_sample = 1.0;
        }
    }
    else if (max_sample_after_gain > ceiling)
    {

        /*--------------------------------------------------------------------------------*
         * Need to decrease gain. Use release_time to calculate gain change.
         **-------------------------------------------------------------------------------*/
        float release_time_samples = release_time * this->graph->get_sample_rate();
        float gain_change = ceiling / max_sample_after_gain;
        gain_change_per_sample = powf(gain_change, 1.0 / release_time_samples);
    }

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = this->input->out[channel][frame] * this->gain;
        }
        this->gain *= gain_change_per_sample;
    }
}

}
