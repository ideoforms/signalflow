#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/granulation/grainsegments.h"

#include <stdlib.h>

namespace signalflow
{

SegmentedGranulator::SegmentedGranulator(BufferRef buffer,
                                         std::vector<float> onset_times,
                                         std::vector<float> durations,
                                         NodeRef index,
                                         NodeRef rate,
                                         NodeRef clock,
                                         NodeRef max_grains)
    : buffer(buffer), onset_times(onset_times), durations(durations), index(index), rate(rate), clock(clock), max_grains(max_grains)
{
    this->name = "segmented-granulator";
    this->rate_scale_factor = 1.0;

    this->create_input("index", this->index);
    this->create_input("rate", this->rate);
    this->create_input("clock", this->clock);
    this->create_input("max_grains", this->max_grains);

    if (onset_times.size() != durations.size())
    {
        throw std::runtime_error("Sizes of onset_times and durations must be equal");
    }
    if (onset_times.size() == 0)
    {
        throw std::runtime_error("At least one onset_time must be specified");
    }

    this->create_buffer("buffer", this->buffer);
    if (buffer)
    {
        this->set_buffer("buffer", buffer);
    }

    this->envelope = new EnvelopeBuffer("triangle");
    this->create_buffer("envelope", envelope);
}

void SegmentedGranulator::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->set_channels(1, buffer->get_num_channels());
        this->rate_scale_factor = buffer->get_sample_rate() / graph->get_sample_rate();
    }
    this->Node::set_buffer(name, buffer);
}

void SegmentedGranulator::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
    {
        return;
    }

    for (int frame = 0; frame < num_frames; frame++)
    {
        int index = (int) this->index->out[0][frame];
        if (index > this->onset_times.size())
        {
            throw std::runtime_error("Invalid segment index: " + std::to_string(index) + " (num_segments = " + std::to_string(this->onset_times.size()) + ")");
        }
        sample onset_time = this->onset_times[index];
        sample duration = this->durations[index];
        sample rate = this->rate->out[0][frame];
        sample max_grains = this->max_grains->out[0][frame];

        if (SIGNALFLOW_CHECK_TRIGGER(this->clock, frame))
        {
            if (this->grains.size() < max_grains)
            {
                Grain *grain = new Grain(buffer,
                                         onset_time * buffer->get_sample_rate(),
                                         duration * buffer->get_sample_rate(),
                                         rate * this->rate_scale_factor);
                this->grains.push_back(grain);
            }
        }

        for (int channel = 0; channel < this->num_output_channels; channel++)
            out[channel][frame] = 0.0;

        std::vector<Grain *>::iterator it;
        for (it = this->grains.begin(); it < this->grains.end();)
        {
            Grain *grain = *it;
            if (!grain->is_finished())
            {
                /*------------------------------------------------------------------------
                 * Step forward in the grain and apply envelope.
                 *-----------------------------------------------------------------------*/
                grain->step();
                float amp = this->envelope->get(0, grain->get_progress());

                for (int channel = 0; channel < this->num_output_channels; channel++)
                {
                    out[channel][frame] += this->buffer->get(channel, grain->phase) * amp;
                }

                it++;
            }
            else
            {
                delete grain;
                grains.erase(it);
            }
        }
    }
}

void SegmentedGranulator::trigger(std::string name, float value)
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
                // CREATE GRAIN
            }
        }
    }
}

}
