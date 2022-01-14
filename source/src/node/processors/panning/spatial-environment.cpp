#include "signalflow/node/processors/panning/spatial-environment.h"

namespace signalflow
{

SpatialSpeaker::SpatialSpeaker(unsigned int channel,
                               float x,
                               float y,
                               float z)
    : channel(channel), x(x), y(y), z(z)
{
}

SpatialEnvironment::SpatialEnvironment()
{
}

std::shared_ptr<SpatialSpeaker> SpatialEnvironment::add_speaker(unsigned int channel,
                                                                float x,
                                                                float y,
                                                                float z)
{
    auto speaker = std::make_shared<SpatialSpeaker>(channel, x, y, z);
    this->speakers.push_back(speaker);

    if (speaker->channel < this->channels.size())
    {
        this->channels[speaker->channel] = speaker;
    }
    else
    {
        this->channels.resize(speaker->channel + 1);
        this->channels[speaker->channel] = speaker;
    }
    return speaker;
}

std::vector<std::shared_ptr<SpatialSpeaker>> SpatialEnvironment::get_speakers()
{
    return speakers;
}

std::vector<std::shared_ptr<SpatialSpeaker>> SpatialEnvironment::get_channels()
{
    return channels;
}

SpatialPanner::SpatialPanner(std::shared_ptr<SpatialEnvironment> env,
                             NodeRef input, NodeRef x, NodeRef y, NodeRef z, NodeRef radius)
    : env(env), input(input), x(x), y(y), z(z), radius(radius)
{
    this->name = "spatial-panner";

    if (this->env)
    {
        auto channels = this->env->get_channels();
        this->set_channels(1, channels.size());
    }

    this->create_input("input", this->input);
    this->create_input("x", this->x);
    this->create_input("y", this->y);
    this->create_input("z", this->z);
    this->create_input("radius", this->radius);
}

void SpatialPanner::process(Buffer &out, int num_frames)
{
    auto speakers = this->env->get_channels();

    for (int frame = 0; frame < num_frames; frame++)
    {
        float input = this->input->out[0][frame];
        float radius = this->radius->out[0][frame];

        for (int channel = 0; channel < this->get_num_output_channels(); channel++)
        {
            float x = this->x->out[0][frame];
            float y = this->y->out[0][frame];
            float z = this->z->out[0][frame];

            auto speaker = speakers[channel];
            if (speaker)
            {
                float distance = sqrtf(powf(speaker->x - x, 2) + powf(speaker->y - y, 2) + powf(speaker->z - z, 2));
                float amp = (radius - distance) / radius;
                if (amp < 0)
                    amp = 0;
                out[channel][frame] = amp * input;
            }
        }
    }
}

}
