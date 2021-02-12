#include "signalflow/core/graph.h"
#include "signalflow/node/analysis/onset-detector.h"

#include <stdlib.h>

namespace signalflow
{

OnsetDetector::OnsetDetector(NodeRef input, NodeRef threshold, NodeRef min_interval)
    : UnaryOpNode(input), threshold(threshold), min_interval(min_interval)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "onset-detector";

    this->create_input("threshold", this->threshold);
    this->create_input("min_interval", this->min_interval);

    this->fast_value = 0.0;
    this->slow_value = 0.0;

    this->fast_lag = 0.99;
    this->slow_lag = 0.999;
    this->interval_timer = 0.0;
}

void OnsetDetector::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float sq = this->input->out[0][frame] * this->input->out[0][frame];
        this->fast_value = (this->fast_lag * this->fast_value) + ((1.0 - this->fast_lag) * sq);
        this->slow_value = (this->slow_lag * this->slow_value) + ((1.0 - this->slow_lag) * sq);

        float rv = 0.0;

        if (this->interval_timer > 0)
        {
            this->interval_timer -= 1;
        }
        else if (this->fast_value > this->slow_value * this->threshold->out[0][frame])
        {
            printf("%f > %f * %f\n", this->fast_value, this->slow_value, this->threshold->out[0][frame]);
            rv = 1.0;
            this->interval_timer = this->min_interval->out[0][frame] * this->graph->get_sample_rate();
        }

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = rv;
        }
    }
}

}
