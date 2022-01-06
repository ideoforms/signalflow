#include "signalflow/node/processors/panning/spatial-environment.h"

namespace signalflow
{

SpatialSpeaker::SpatialSpeaker(unsigned int channel,
                               float x, float y)
    : channel(channel), x(x), y(y)
{
}

SpatialEnvironment::SpatialEnvironment()
{
}

std::shared_ptr<SpatialSpeaker> SpatialEnvironment::add_speaker(unsigned int channel, float x, float y)
{
    auto speaker = std::make_shared<SpatialSpeaker>(channel, x, y);
    this->speakers.push_back(speaker);
    return speaker;
}

std::vector<std::shared_ptr<SpatialSpeaker>> SpatialEnvironment::get_speakers()
{
    return speakers;
}

SpatialPanner::SpatialPanner(std::shared_ptr<SpatialEnvironment> env,
                             NodeRef input, NodeRef x, NodeRef y, NodeRef radius)
    : env(env), input(input), x(x), y(y), radius(radius)
{
    this->name = "spatial-panner";

    auto speakers = this->env->get_speakers();
    this->set_channels(1, speakers.size());

    this->create_input("input", this->input);
    this->create_input("x", this->x);
    this->create_input("y", this->y);
    this->create_input("radius", this->radius);
}

void SpatialPanner::process(Buffer &out, int num_frames)
{
    auto speakers = this->env->get_speakers();

    for (int frame = 0; frame < num_frames; frame++)
    {
        float input = this->input->out[0][frame];
        float radius = this->radius->out[0][frame];

        for (int channel = 0; channel < this->get_num_output_channels(); channel++)
        {
            float x = this->x->out[0][frame];
            float y = this->y->out[0][frame];

            auto speaker = speakers[channel];
            float distance = sqrtf(powf(speaker->x - x, 2) + powf(speaker->y - y, 2));
            float amp = (radius - distance) / radius;
            if (amp < 0)
                amp = 0;
            out[channel][frame] = amp * input;
        }
    }
}

}
