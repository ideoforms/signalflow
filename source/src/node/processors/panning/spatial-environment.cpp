#include "signalflow/core/graph.h"
#include "signalflow/node/processors/panning/spatial-environment.h"
#include <limits>

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
                             NodeRef input, NodeRef x, NodeRef y, NodeRef z, NodeRef radius,
                             NodeRef use_delays,
                             std::string algorithm)
    : env(env), input(input), x(x), y(y), z(z), radius(radius), use_delays(use_delays), algorithm(algorithm)
{
    this->name = "spatial-panner";

    if (this->env)
    {
        auto channels = this->env->get_channels();
        this->set_channels(1, channels.size());
    }

    // 1.0 = max_delay_time
    this->buffer = new SampleRingBuffer(1.0 * this->graph->get_sample_rate());

    this->create_input("input", this->input);
    this->create_input("x", this->x);
    this->create_input("y", this->y);
    this->create_input("z", this->z);
    this->create_input("radius", this->radius);
    this->create_input("use_delays", this->use_delays);

    if (algorithm != "dbap" && algorithm != "nearest" && algorithm != "beamformer")
    {
        throw std::runtime_error("Invalid spatialisation algorithm: " + algorithm);
    }
}

void SpatialPanner::process(Buffer &out, int num_frames)
{
    auto speakers = this->env->get_channels();

    if (algorithm == "dbap")
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            for (int channel = 0; channel < this->get_num_output_channels(); channel++)
            {
                auto speaker = speakers[channel];
                if (speaker)
                {
                    float distance = sqrtf(powf(speaker->x - this->x->out[0][frame], 2) + powf(speaker->y - this->y->out[0][frame], 2) + powf(speaker->z - this->z->out[0][frame], 2));

                    float amp = (this->radius->out[0][frame] - distance) / this->radius->out[0][frame];
                    if (amp < 0)
                        amp = 0;

                    out[channel][frame] = amp * this->input->out[0][frame];
                }
            }
        }
    }
    else if (algorithm == "nearest")
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            unsigned int nearest_index = -1;
            float nearest_distance = std::numeric_limits<float>::max();
            for (int channel = 0; channel < this->get_num_output_channels(); channel++)
            {
                auto speaker = speakers[channel];
                if (speaker)
                {
                    float distance = sqrtf(powf(speaker->x - this->x->out[0][frame], 2) + powf(speaker->y - this->y->out[0][frame], 2) + powf(speaker->z - this->z->out[0][frame], 2));

                    if (distance < nearest_distance)
                    {
                        nearest_distance = distance;
                        nearest_index = channel;
                    }
                    out[channel][frame] = 0.0;
                }
            }
            out[nearest_index][frame] = this->input->out[0][frame];
        }
    }
    else if (algorithm == "beamformer")
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            this->buffer->append(this->input->out[0][frame]);
            for (int channel = 0; channel < this->get_num_output_channels(); channel++)
            {
                auto speaker = speakers[channel];
                if (speaker)
                {
                    // m/s
                    float SPEED_OF_SOUND = 343.0;
                    float distance = sqrtf(powf(speaker->x - this->x->out[0][frame], 2) + powf(speaker->y - this->y->out[0][frame], 2) + powf(speaker->z - this->z->out[0][frame], 2));

                    //                    float amp = (this->radius->out[0][frame] - distance) / this->radius->out[0][frame];
                    //                    if (amp < 0)
                    //                        amp = 0;
                    float distance_m = distance * 0.001;
                    float attenuation = 1.0 / (distance_m * distance_m);
                    //                    float attenuation = 1.0;
                    if (frame == 0 && channel == 0)
                    {
                        //                        printf("distance %.4f, attenuation: %.4f\n", distance, attenuation);
                        //                        printf("use_delays: %f\n", use_delays->out[0][frame]);
                    }

                    if (use_delays->out[0][frame])
                    {
                        float delay_seconds = distance_m / SPEED_OF_SOUND;
                        float delay_samples = delay_seconds * this->graph->get_sample_rate();
                        out[channel][frame] = this->buffer->get(-delay_samples) * attenuation;
                    }
                    else
                    {
                        out[channel][frame] = this->buffer->get(-1) * attenuation;
                    }
                }
            }
        }
    }
}

}
