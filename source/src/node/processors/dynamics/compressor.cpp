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
    this->current_ratio = 1.0;

    this->create_input("threshold", this->threshold);
    this->create_input("ratio", this->ratio);
    this->create_input("attack_time", this->attack_time);
    this->create_input("release_time", this->release_time);
    this->create_input("sidechain", this->sidechain);
}

void Compressor::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float input_sample = this->sidechain ? this->sidechain->out[0][frame] : this->input->out[0][frame];
        if (fabsf(input_sample) > fabsf(this->threshold->out[0][frame]))
        {
            if (this->current_ratio < this->ratio->out[0][frame])
            {
                float attack_time_samples = this->attack_time->out[0][frame] * this->graph->get_sample_rate();
                float compression_change_per_sample = ((this->ratio->out[0][frame] - 1) / attack_time_samples);
                this->current_ratio += compression_change_per_sample;
                // check overshoot
                if (this->current_ratio > this->ratio->out[0][frame])
                {
                    this->current_ratio = this->ratio->out[0][frame];
                }
            }
        }
        else
        {
            if (this->current_ratio > 1)
            {
                float release_time_samples = this->release_time->out[0][frame] * this->graph->get_sample_rate();
                float compression_change_per_sample = (-(this->ratio->out[0][frame] - 1) / release_time_samples);
                this->current_ratio += compression_change_per_sample;
                if (this->current_ratio < 1)
                {
                    this->current_ratio = 1;
                }
            }
        }

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = this->input->out[channel][frame] / this->current_ratio;
            //            if (fabsf(this->input->out[channel][frame]) < threshold->out[channel][frame])
            //            {
            //                out[channel][frame] = this->input->out[channel][frame];
            //            }
            //            else
            //            {
            //                float remainder = fabsf(this->input->out[channel][frame]) - threshold->out[channel][frame];
            //                float remainder_compressed = remainder / this->current_ratio;
            //                float sample = threshold->out[channel][frame] + remainder_compressed;
            //                printf("ratio %f, remainder %f, sample %f\n", this->current_ratio, remainder_compressed, sample);
            //                out[channel][frame] = (this->input->out[channel][frame] > 0) ? sample : -sample;
            //                out[channel][frame] = this->input->out[channel][frame] / this->current_ratio;
            //            }
        }
    }
}

}
