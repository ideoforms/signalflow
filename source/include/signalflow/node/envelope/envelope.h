#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Generic envelope constructor, given arrays of levels, times and curves that
 * describe the shape of the envelope over time.
 * 
 * Note that there must be one fewer time than levels, and the curves array
 * should match the number of segments between levels.
 *---------------------------------------------------------------------------------*/
class Envelope : public Node
{
public:
    Envelope(std::vector<NodeRef> levels = std::vector<NodeRef>(),
             std::vector<NodeRef> times = std::vector<NodeRef>(),
             std::vector<NodeRef> curves = std::vector<NodeRef>(),
             NodeRef clock = nullptr,
             bool loop = false);

    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = SIGNALFLOW_NULL_FLOAT) override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    float level;
    unsigned int node_index;
    float node_phase;
    std::vector<NodeRef> levels;
    std::vector<NodeRef> times;
    std::vector<NodeRef> curves;
    NodeRef clock;
    bool loop;
};

REGISTER(Envelope, "envelope")

}
