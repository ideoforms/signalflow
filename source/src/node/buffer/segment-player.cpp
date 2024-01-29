#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"
#include "signalflow/node/buffer/segment-player.h"

#include <stdlib.h>

namespace signalflow
{

SegmentPlayer::SegmentPlayer(BufferRef buffer, std::vector<float> onsets, NodeRef index, NodeRef rate, NodeRef clock)
    : buffer(buffer), index(index), rate(rate), clock(clock)
{
    this->name = "segment-player";

    this->set_channels(1, buffer->get_num_channels());
    this->create_buffer("buffer", buffer);
    this->create_property("onsets", this->onsets);
    this->set_property("onsets", onsets);
    this->create_input("index", this->index);
    this->create_input("rate", this->rate);
    this->create_input("clock", this->clock);

    this->rate_scale_factor = 1.0;
    this->phase = 0.0;

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

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if ((unsigned int) this->phase < buffer->get_num_frames())
            {
                s = this->buffer->get_frame(channel, this->phase);
            }
            else
            {
                s = 0.0;
            }

            out[channel][frame] = s;
        }

        this->phase += this->rate->out[0][frame] * this->rate_scale_factor;
    }
}

void SegmentPlayer::trigger(std::string name, float value)
{
    /*--------------------------------------------------------------------------------
     * TODO: How to honour `value` here, if specified?
     *       Perhaps the default should be a null value, that gets ignored in favour
     *       of `index` if not specified.
     *--------------------------------------------------------------------------------*/
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        PropertyRef onsetsref = this->get_property("onsets");
        if (onsetsref)
        {
            std::vector<float> onsets = onsetsref->float_array_value();
            if (onsets.size() > 0)
            {
                int segment_index;
                if (this->index)
                {
                    segment_index = this->index->out[0][0];
                }
                else
                {
                    segment_index = random_integer(0, onsets.size());
                }
                this->phase = onsets[segment_index] * this->buffer->get_sample_rate();
            }
        }
    }
}

}
