#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class SpatialSpeaker
{
public:
    SpatialSpeaker(unsigned int channel, float x, float y);

    unsigned int channel;
    float x;
    float y;
};

class SpatialEnvironment
{
public:
    SpatialEnvironment();
    std::shared_ptr<SpatialSpeaker> add_speaker(unsigned int channel, float x, float y);
    std::vector<std::shared_ptr<SpatialSpeaker>> get_speakers();

private:
    std::vector<std::shared_ptr<SpatialSpeaker>> speakers;
};

class SpatialPanner : public Node
{
public:
    SpatialPanner(std::shared_ptr<SpatialEnvironment> env = nullptr,
                  NodeRef input = 0.0,
                  NodeRef x = 0.0,
                  NodeRef y = 0.0,
                  NodeRef radius = 1.0);

    virtual void process(Buffer &out, int num_frames) override;

    std::shared_ptr<SpatialEnvironment> env;
    NodeRef input;
    NodeRef x;
    NodeRef y;
    NodeRef radius;
};

REGISTER(SpatialPanner, "spatial-panner")
}
