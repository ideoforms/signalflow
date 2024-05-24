#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Represents a speaker in a SpatialEnvironment.
 *---------------------------------------------------------------------------------*/
class SpatialSpeaker
{
public:
    SpatialSpeaker(unsigned int channel, float x, float y, float z = 0.0);

    unsigned int channel;
    float x;
    float y;
    float z;
};

/**--------------------------------------------------------------------------------*
 * Encapsulates a 3D environment containing speakers and sound sources, used
 * in a SpatialPanner.
 *---------------------------------------------------------------------------------*/
class SpatialEnvironment
{
public:
    SpatialEnvironment();
    std::shared_ptr<SpatialSpeaker> add_speaker(unsigned int channel, float x, float y, float z = 0.0);
    std::vector<std::shared_ptr<SpatialSpeaker>> get_speakers();
    std::vector<std::shared_ptr<SpatialSpeaker>> get_channels();

private:
    std::vector<std::shared_ptr<SpatialSpeaker>> speakers;
    std::vector<std::shared_ptr<SpatialSpeaker>> channels;
};

/**--------------------------------------------------------------------------------*
 * Implements a spatial panning algorithm, applied to a given SpatialEnvironment.
 * Currently, only DBAP is supported.
 *---------------------------------------------------------------------------------*/
class SpatialPanner : public Node
{
public:
    SpatialPanner(std::shared_ptr<SpatialEnvironment> env = nullptr,
                  NodeRef input = 0.0,
                  NodeRef x = 0.0,
                  NodeRef y = 0.0,
                  NodeRef z = 0.0,
                  NodeRef radius = 1.0,
                  NodeRef use_delays = 1.0,
                  std::string algorithm = "dbap");

    virtual void process(Buffer &out, int num_frames) override;

    std::shared_ptr<SpatialEnvironment> env;
    NodeRef input;
    NodeRef x;
    NodeRef y;
    NodeRef z;
    NodeRef radius;
    NodeRef use_delays;
    std::string algorithm;
    SampleRingBuffer *buffer;
};

REGISTER(SpatialPanner, "spatial-panner")
}
