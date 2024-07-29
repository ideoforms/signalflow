#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"
#include "signalflow/node/buffer/segment-player.h"

#include <stdlib.h>

namespace signalflow
{

SegmentPlayer::SegmentPlayer(BufferRef buffer, std::vector<float> onsets, NodeRef index, NodeRef rate,
                             NodeRef start_offset, NodeRef clock, NodeRef continue_after_segment)
    : buffer(buffer), index(index), rate(rate), start_offset(start_offset), clock(clock), continue_after_segment(continue_after_segment)
{
    this->name = "segment-player";

    this->set_channels(1, buffer->get_num_channels());
    this->create_buffer("buffer", buffer);
    this->create_property("onsets", this->onsets);
    this->set_property("onsets", onsets);
    this->create_input("index", this->index);
    this->create_input("rate", this->rate);
    this->create_input("start_offset", this->start_offset);
    this->create_input("clock", this->clock);
    this->create_input("continue_after_segment", this->continue_after_segment);

    this->rate_scale_factor = 1.0;
    this->phase = 0.0;
    this->segment_end_phase = 0.0;

    if (buffer)
    {
        this->set_buffer("buffer", buffer);
    }

    this->trigger();
}

void SegmentPlayer::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->num_output_channels = buffer->get_num_channels();
        this->rate_scale_factor = buffer->get_sample_rate() / graph->get_sample_rate();
    }
    this->Node::set_buffer(name, buffer);
}

void SegmentPlayer::process(Buffer &out, int num_frames)
{
    sample s;

    int continue_after_segment = this->continue_after_segment->out[0][0];
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if (this->state == SIGNALFLOW_NODE_STATE_STOPPED)
            {
                s = 0.0;
            }
            else
            {
                if ((unsigned int) this->phase < buffer->get_num_frames())
                {
                    s = this->buffer->get_frame(channel, this->phase);
                }
                else
                {
                    s = 0.0;
                }
            }

            out[channel][frame] = s;
        }

        this->phase += this->rate->out[0][frame] * this->rate_scale_factor;
        if (!continue_after_segment && (this->phase >= this->segment_end_phase))
        {
            this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
        }
    }
}

void SegmentPlayer::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        PropertyRef onsetsref = this->get_property("onsets");

        if (onsetsref)
        {
            std::vector<float> onsets = onsetsref->float_array_value();
            if (onsets.size() > 0)
            {
                int segment_index;
                /*--------------------------------------------------------------------------------
                 * If `value` is explicitly specified, use this as the segment index.
                 * Otherwise, use the value of the `index` input, or a random value.
                 *--------------------------------------------------------------------------------*/
                if (value != SIGNALFLOW_NULL_FLOAT)
                {
                    segment_index = (int) value;
                }
                else if (this->index)
                {
                    segment_index = this->index->out[0][0];
                }
                else
                {
                    segment_index = random_integer(0, onsets.size());
                }

                float start_offset = 0.0f;
                if (this->start_offset)
                {
                    start_offset = this->start_offset->out[0][0];
                }

                this->phase = (onsets[segment_index] + start_offset) * this->buffer->get_sample_rate();
                if (segment_index < onsets.size() - 1)
                {
                    this->segment_end_phase = onsets[segment_index + 1] * this->buffer->get_sample_rate();
                }
                else
                {
                    this->segment_end_phase = this->buffer->get_duration() * this->buffer->get_sample_rate();
                }

                this->set_state(SIGNALFLOW_NODE_STATE_ACTIVE);
            }
        }
    }
}

}
