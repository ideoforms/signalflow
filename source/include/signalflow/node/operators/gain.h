#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Apply a gain to the input, specified in decibels.
 *---------------------------------------------------------------------------------*/
class Gain : public UnaryOpNode
{
public:
    Gain(NodeRef input = 0.0, NodeRef gain = 0.0);

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef gain;

    /*--------------------------------------------------------------------------------
     * The most recent gain value and its linear amplitude, to avoid recalculating
     * the amplitude when the gain is unchanged.
     *-------------------------------------------------------------------------------*/
    sample last_gain_db = 0.0;
    sample last_gain_amplitude = 1.0;
};

REGISTER(Gain, "gain")

}
