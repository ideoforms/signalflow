#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/envelope.h"

namespace signalflow
{

Envelope::Envelope(std::vector<NodeRef> levels,
                   std::vector<NodeRef> times)
    : levels(levels), times(times)
{
    this->level = -1;
    this->node_index = 0;
    this->node_phase = 0.0;
    this->set_channels(1, 1);

    if (levels.size() < 2 || times.size() < 1 || (levels.size() != times.size() + 1))
    {
        throw std::runtime_error("Invalid levels/times combination");
    }

    this->name = "envelope";
    for (int i = 0; i < levels.size(); i++)
    {
        std::string input_name = "levels" + std::to_string(i);
        this->create_input(input_name, this->levels[i]);
    }
    for (int i = 0; i < times.size(); i++)
    {
        std::string input_name = "times" + std::to_string(i);
        this->create_input(input_name, this->times[i]);
    }
}

void Envelope::process(Buffer &out, int num_frames)
{
    if (level == -1)
    {
        level = this->levels[0]->out[0][0];
    }

    float phase_step = 1.0f / this->graph->get_sample_rate();

    for (int frame = 0; frame < num_frames; frame++)
    {
        if (node_index < levels.size() - 1)
        {
            float level_target = this->levels[node_index + 1]->out[0][frame];
            float time = this->times[node_index]->out[0][frame];
            float time_remaining = time - this->node_phase;
            int steps_remaining = time_remaining * graph->get_sample_rate();
            if (steps_remaining <= 0)
            {
                level = level_target;
                this->node_phase = 0.0;
                this->node_index++;
            }
            else
            {
                float step = (level_target - level) / steps_remaining;
                level += step;
                printf("%f\n", level);
                this->node_phase += phase_step;
            }
        }
        this->out[0][frame] = level;
    }
}

}
