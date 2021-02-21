#include "signalflow/node/buffer/granulator.h"

namespace signalflow
{

Granulator::Granulator(BufferRef buffer, NodeRef clock, NodeRef pos, NodeRef duration, NodeRef pan, NodeRef rate, NodeRef max_grains)
    : buffer(buffer), pos(pos), clock(clock), duration(duration), pan(pan), rate(rate), max_grains(max_grains)
{
    this->name = "granulator";

    this->create_input("pos", this->pos);
    this->create_input("clock", this->clock);
    this->create_input("duration", this->duration);
    this->create_input("pan", this->pan);
    this->create_input("rate", this->rate);
    this->create_input("max_grains", this->max_grains);

    this->create_buffer("buffer", buffer);

    this->envelope = new Buffer("triangle", 1024);
    this->create_buffer("envelope", envelope);

    this->set_channels(1, 2);
    this->create_input("pan", this->pan);

    this->clock_last = 0.0;
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
                sample s = this->buffer->get(buffer_index);

                /*------------------------------------------------------------------------
                 * Apply grain envelope.
                 *-----------------------------------------------------------------------*/
                float env_phase = (float) grain->samples_done / grain->sample_length;
                float amp = this->envelope->get(env_phase * this->envelope->get_num_frames());

                grain->samples_done += grain->rate;

                /*------------------------------------------------------------------------
                 * Calculate pan.
                 * TODO: Handle >2 channels
                 *-----------------------------------------------------------------------*/
                float rv = s * amp;
                out[0][frame] += rv * (1.0 - 0.5 * (grain->pan + 1));
                out[1][frame] += rv * (0.5 * (grain->pan + 1));

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
