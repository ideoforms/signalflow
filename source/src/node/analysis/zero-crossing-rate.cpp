#include "signalflow/core/graph.h"
#include "signalflow/node/analysis/zero-crossing-rate.h"

#include <cmath>

namespace signalflow
{

ZeroCrossingRate::ZeroCrossingRate(NodeRef input)
    : UnaryOpNode(input)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "zero-crossing-rate";
}

void ZeroCrossingRate::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        int crossings = 0;
        float prev = this->input->last_sample[channel];

        for (int frame = 0; frame < num_frames; frame++)
        {
            float current = this->input->out[channel][frame];

            // Detect zero crossing: sign change between consecutive samples
            if (prev < 0.0f && current >= 0.0f)
            {
                crossings++;
            }

            prev = current;
        }

        // Normalize by number of frames to get rate (crossings per sample)
        float zcr = (float) crossings / (float) num_frames;

        // Fill output buffer with the calculated ZCR
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = zcr;
        }
    }
}

}
