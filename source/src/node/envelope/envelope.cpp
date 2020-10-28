#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/envelope.h"

namespace signalflow
{

Envelope::Envelope(std::vector<NodeRef> levels,
                   std::vector<NodeRef> times,
                   std::vector<NodeRef> curves)
    : levels(levels), times(times), curves(curves)
{
    this->level = std::numeric_limits<float>::max();
    this->node_index = 0;
    this->node_phase = 0.0;
    this->set_channels(1, 1);

    if (levels.size() < 2 || times.size() < 1 || (levels.size() != times.size() + 1))
    {
        throw std::runtime_error("Invalid levels/times combination");
    }
    if (curves.size() > 0 && curves.size() != times.size())
    {
        throw std::runtime_error("Invalid curves/times combination");
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
    for (int i = 0; i < curves.size(); i++)
    {
        std::string input_name = "curves" + std::to_string(i);
        this->create_input(input_name, this->curves[i]);
    }
}

void Envelope::process(Buffer &out, int num_frames)
{
    if (level == std::numeric_limits<float>::max())
    {
        level = this->levels[0]->out[0][0];
    }

    float phase_step = 1.0f / this->graph->get_sample_rate();
    float rv = level;

    for (int frame = 0; frame < num_frames; frame++)
    {
        if (node_index < levels.size() - 1)
        {
            float level_target = this->levels[node_index + 1]->out[0][frame];
            float time = this->times[node_index]->out[0][frame];
            float curve = (this->curves.size() > 0) ? (this->curves[node_index]->out[0][frame]) : 1;
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
                this->node_phase += phase_step;
            }

            rv = powf(level, curve);
        }

        this->out[0][frame] = rv;
    }
}

}
