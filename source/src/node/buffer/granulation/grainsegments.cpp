#include "signalflow/buffer/envelope-buffer.h"
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
        if (SIGNALFLOW_CHECK_TRIGGER(this->clock, frame) || this->triggered)
        {
            this->triggered = false;
            int index = (int) this->index->out[0][frame];
            if (index < 0 || index >= this->onset_times.size())
            {
                throw std::runtime_error("Invalid segment index: " + std::to_string(index) + " (num_segments = " + std::to_string(this->onset_times.size()) + ")");
            }

            if (this->grains.size() < this->max_grains->out[0][frame])
            {
                Grain *grain = new Grain(buffer,
                                         this->onset_times[index] * buffer->get_sample_rate(),
                                         this->durations[index] * buffer->get_sample_rate(),
                                         this->rate->out[0][frame] * this->rate_scale_factor);
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
                float amp = this->envelope->get(0, grain->get_progress());
                grain->step();

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
     * TODO: Don't repeat this same block of code from the trigger block above
     *--------------------------------------------------------------------------------*/
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        if (value == SIGNALFLOW_NULL_FLOAT)
        {
            /*--------------------------------------------------------------------------------
             * If triggered without an explicitly-specified index, defer the trigger
             * until the next process() block. This is because, if the .index property of
             * the granulator is set at the same time as trigger() is called, the .index
             * update won't take effect until the next process().
             *--------------------------------------------------------------------------------*/
            this->triggered = true;
            return;
        }

        int index = (int) value;
        if (index < 0 || index >= this->onset_times.size())
        {
            throw std::runtime_error("Invalid segment index: " + std::to_string(index) + " (num_segments = " + std::to_string(this->onset_times.size()) + ")");
        }

        if (this->grains.size() < this->max_grains->out[0][0])
        {
            Grain *grain = new Grain(buffer,
                                     this->onset_times[index] * buffer->get_sample_rate(),
                                     this->durations[index] * buffer->get_sample_rate(),
                                     this->rate->out[0][0] * this->rate_scale_factor);
            this->grains.push_back(grain);
        }
    }
}

}
