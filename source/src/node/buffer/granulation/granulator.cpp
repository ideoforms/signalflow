#include "signalflow/buffer/envelope-buffer.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/granulation/granulator.h"

namespace signalflow
{

Granulator::Granulator(BufferRef buffer,
                       NodeRef clock,
                       NodeRef pos,
                       NodeRef duration,
                       NodeRef amplitude,
                       NodeRef pan,
                       NodeRef rate,
                       NodeRef max_grains,
                       bool wrap)
    : pos(pos), clock(clock), duration(duration), amplitude(amplitude), pan(pan), rate(rate), max_grains(max_grains), wrap(wrap)
{
    this->name = "granulator";

    this->create_input("pos", this->pos);
    this->create_input("clock", this->clock);
    this->create_input("duration", this->duration);
    this->create_input("amplitude", this->amplitude);
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

    this->create_input("pan", this->pan);
}

void Granulator::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->set_channels(1, 2);
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
    {
        return;
    }

    for (int frame = 0; frame < num_frames; frame++)
    {
        sample pos = this->pos->out[0][frame];
        sample duration = this->duration->out[0][frame];
        sample rate = this->rate->out[0][frame];
        sample amplitude = this->amplitude->out[0][frame];
        sample pan = this->pan->out[0][frame];
        sample max_grains = this->max_grains->out[0][frame];

        if (SIGNALFLOW_CHECK_TRIGGER(clock, frame))
        {
            if (this->grains.size() < max_grains)
            {
                Grain *grain = new Grain(buffer,
                                         pos * buffer->get_sample_rate(),
                                         duration * buffer->get_sample_rate(),
                                         rate * this->rate_scale_factor,
                                         amplitude,
                                         pan,
                                         this->wrap);
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
                 * Query the amplitude before stepping forward in the grain so that we
                 * obtain the amplitude sample for the correct index (e.g, phase = 0
                 * for the first sample).
                 *-----------------------------------------------------------------------*/
                float amp = this->envelope->get(0, grain->get_progress()) * grain->amplitude;
                grain->step();

                /*------------------------------------------------------------------------
                 * Calculate pan.
                 * If input buffer is mono, pan to L/R.
                 * If input buffer is stereo, apply stereo balance.
                 * TODO: Handle >2 channels
                 * TODO: Handle <2 channels
                 *-----------------------------------------------------------------------*/
                if (this->buffer->get_num_channels() == 1)
                {
                    sample s = this->buffer->get(0, grain->phase);

                    float rv = s * amp;
                    out[0][frame] += rv * (1.0 - 0.5 * (grain->pan + 1));
                    out[1][frame] += rv * (0.5 * (grain->pan + 1));
                }
                else if (this->buffer->get_num_channels() == 2)
                {
                    out[0][frame] += this->buffer->get(0, grain->phase) * amp * (1.0 - 0.5 * (grain->pan + 1));
                    out[1][frame] += this->buffer->get(1, grain->phase) * amp * (0.5 * (grain->pan + 1));
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

void Granulator::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        Grain *grain = new Grain(this->buffer,
                                 this->pos->out[0][0] * buffer->get_sample_rate(),
                                 this->duration->out[0][0] * buffer->get_sample_rate(),
                                 this->rate->out[0][0] * this->rate_scale_factor,
                                 this->amplitude->out[0][0],
                                 this->pan->out[0][0],
                                 this->wrap);
        this->grains.push_back(grain);
    }
    else
    {
        this->Node::trigger(name, value);
    }
}

}
