#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/envelope.h"

#include <limits>

namespace signalflow
{

Envelope::Envelope(std::vector<NodeRef> levels,
                   std::vector<NodeRef> times,
                   std::vector<NodeRef> curves,
                   NodeRef clock,
                   bool loop)
    : levels(levels), times(times), curves(curves), clock(clock), loop(loop)
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
    for (size_t i = 0; i < levels.size(); i++)
    {
        std::string input_name = "levels" + std::to_string(i);
        this->create_input(input_name, this->levels[i]);
    }
    for (size_t i = 0; i < times.size(); i++)
    {
        std::string input_name = "times" + std::to_string(i);
        this->create_input(input_name, this->times[i]);
    }
    for (size_t i = 0; i < curves.size(); i++)
    {
        std::string input_name = "curves" + std::to_string(i);
        this->create_input(input_name, this->curves[i]);
    }
    this->create_input("clock", this->clock);

    /*--------------------------------------------------------------------------------
     * If no clock signal is specified, trigger immediately.
     * If a clock is specified, don't trigger until a trigger is received.
     *-------------------------------------------------------------------------------*/
    if (clock)
    {
        this->state = SIGNALFLOW_NODE_STATE_STOPPED;
    }
    else
    {
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
}

void Envelope::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        /*--------------------------------------------------------------------------------
         * If given, value is parsed as the integer index of the node to jump to in the
         * envelope (0 = start of envelope, 1 = first node, etc).
         *-------------------------------------------------------------------------------*/
        int _node_index = (value == SIGNALFLOW_NULL_FLOAT) ? 0 : int(value);
        this->level = std::numeric_limits<float>::max();
        this->node_index = _node_index;
        this->node_phase = 0;
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
}

void Envelope::process(Buffer &out, int num_frames)
{
    float phase_step = 1.0f / this->graph->get_sample_rate();
    float rv = 0.0;

    for (int frame = 0; frame < num_frames; frame++)
    {
        SIGNALFLOW_PROCESS_TRIGGER(this->clock, frame, SIGNALFLOW_DEFAULT_TRIGGER);

        if (level == std::numeric_limits<float>::max())
        {
            level = this->levels[this->node_index]->out[0][frame];
        }

        if (this->state == SIGNALFLOW_NODE_STATE_ACTIVE)
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
            else
            {
                if (this->loop)
                {
                    this->trigger();
                }
                else
                {
                    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
                }
            }
        }

        out[0][frame] = rv;
    }
}

}
