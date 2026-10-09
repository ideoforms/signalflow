#include "signalflow/core/graph.h"
#include "signalflow/node/processors/dynamics/compressor.h"

namespace signalflow
{

Compressor::Compressor(NodeRef input, NodeRef threshold, NodeRef ratio,
                       NodeRef attack_time, NodeRef release_time,
                       NodeRef sidechain)
    : UnaryOpNode(input), threshold(threshold), ratio(ratio), attack_time(attack_time), release_time(release_time), sidechain(sidechain)
{
    this->name = "compressor";
    this->envelope = 0.0;

    this->create_input("threshold", this->threshold);
    this->create_input("ratio", this->ratio);
    this->create_input("attack_time", this->attack_time);
    this->create_input("release_time", this->release_time);
    this->create_input("sidechain", this->sidechain);
}

void Compressor::process(Buffer &out, int num_frames)
{
    float sample_rate = this->graph->get_sample_rate();
    for (int frame = 0; frame < num_frames; frame++)
    {
        /*--------------------------------------------------------------------------------
         * Follow the level of the input (or sidechain) with a peak envelope, rising with
         * attack_time and falling with release_time (one-pole, time constants in seconds).
         *--------------------------------------------------------------------------------*/
        float level = fabsf(this->sidechain ? this->sidechain->out[0][frame] : this->input->out[0][frame]);
        float time = (level > this->envelope) ? this->attack_time->out[0][frame] : this->release_time->out[0][frame];
        float coefficient = (time > 0) ? expf(-1.0f / (time * sample_rate)) : 0.0f;
        this->envelope = level + coefficient * (this->envelope - level);

        /*--------------------------------------------------------------------------------
         * Above the threshold, reduce the gain so that the level above the threshold is
         * divided by the ratio (in dB): gain = (threshold / envelope) ^ (1 - 1 / ratio).
         *--------------------------------------------------------------------------------*/
        float threshold = fabsf(this->threshold->out[0][frame]);
        float ratio = fmaxf(this->ratio->out[0][frame], 1.0f);
        float gain = 1.0f;
        if (this->envelope > threshold && threshold > 0)
        {
            gain = powf(threshold / this->envelope, 1.0f - 1.0f / ratio);
        }

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = this->input->out[channel][frame] * gain;
        }
    }
}

}
