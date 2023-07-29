#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/granulator.h"

namespace signalflow
{

Granulator::Granulator(BufferRef buffer, NodeRef clock, NodeRef pos, NodeRef duration, NodeRef pan, NodeRef rate, NodeRef max_grains)
    : pos(pos), clock(clock), duration(duration), pan(pan), rate(rate), max_grains(max_grains)
{
    this->name = "granulator";

    this->create_input("pos", this->pos);
    this->create_input("clock", this->clock);
    this->create_input("duration", this->duration);
    this->create_input("pan", this->pan);
    this->create_input("rate", this->rate);
    this->create_input("max_grains", this->max_grains);

    this->create_buffer("buffer", this->buffer);
    if (buffer)
    {
        this->set_buffer("buffer", buffer);
    }

    this->envelope = new EnvelopeBuffer("triangle");
    this->create_buffer("envelope", envelope);

    this->set_channels(1, 2);
    this->create_input("pan", this->pan);

    this->clock_last = 0.0;
}

void Granulator::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->rate_scale_factor = buffer->get_sample_rate() / graph->get_sample_rate();
    }
    this->Node::set_buffer(name, buffer);
}

void Granulator::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    for (int frame = 0; frame < num_frames; frame++)
    {
        sample pos = this->pos->out[0][frame];
        sample clock_value = this->clock->out[0][frame];
        sample duration = this->duration->out[0][frame];
        sample rate = this->rate->out[0][frame];
        sample pan = this->pan->out[0][frame];
        sample max_grains = this->max_grains->out[0][frame];

        if (clock_value > clock_last)
        {
            if (this->grains.size() < max_grains)
            {
                Grain *grain = new Grain(buffer, pos * buffer->get_sample_rate(), duration * buffer->get_sample_rate(), rate, pan);
                this->grains.push_back(grain);
            }
        }
        clock_last = clock_value;

        for (int channel = 0; channel < this->num_output_channels; channel++)
            out[channel][frame] = 0.0;

        std::vector<Grain *>::iterator it;
        for (it = this->grains.begin(); it < this->grains.end();)
        {
            Grain *grain = *it;
            if (!grain->finished())
            {
                /*------------------------------------------------------------------------
                 * Obtain the correct sample from the buffer.
                 *-----------------------------------------------------------------------*/
                double buffer_index = grain->sample_start + grain->samples_done;
                while (buffer_index > this->buffer->get_num_frames())
                    buffer_index -= this->buffer->get_num_frames();

                /*------------------------------------------------------------------------
                 * Apply grain envelope.
                 *-----------------------------------------------------------------------*/
                float env_phase = (float) grain->samples_done / grain->sample_length;
                float amp = this->envelope->get(0, env_phase);

                grain->samples_done += grain->rate * this->rate_scale_factor;

                /*------------------------------------------------------------------------
                 * Calculate pan.
                 * If input buffer is mono, pan to L/R.
                 * If input buffer is stereo, apply stereo balance.
                 * TODO: Handle >2 channels
                 * TODO: Handle <2 channels
                 *-----------------------------------------------------------------------*/
                if (this->buffer->get_num_channels() == 1)
                {
                    sample s = this->buffer->get(0, buffer_index);

                    float rv = s * amp;
                    out[0][frame] += rv * (1.0 - 0.5 * (grain->pan + 1));
                    out[1][frame] += rv * (0.5 * (grain->pan + 1));
                }
                else if (this->buffer->get_num_channels() == 2)
                {
                    out[0][frame] += this->buffer->get(0, buffer_index) * amp * (1.0 - 0.5 * (grain->pan + 1));
                    out[1][frame] += this->buffer->get(1, buffer_index) * amp * (0.5 * (grain->pan + 1));
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

Grain::Grain(BufferRef buffer, int start, int length, float rate, float pan)
    : buffer(buffer), sample_start(start), sample_length(length), rate(rate), pan(pan)
{
    this->samples_done = 0;
}

bool Grain::finished()
{
    return this->samples_done >= this->sample_length;
}

}
