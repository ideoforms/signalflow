#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/wavetable.h"

namespace signalflow
{

Wavetable::Wavetable(BufferRef buffer, NodeRef frequency, NodeRef phase, NodeRef sync, BufferRef phase_map)
    : buffer(buffer), frequency(frequency), phase(phase), sync(sync), phase_map(phase_map)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "wavetable";

    this->create_input("frequency", this->frequency);
    this->create_input("phase", this->phase);
    this->create_input("sync", this->sync);
    this->create_buffer("buffer", this->buffer);
    this->create_buffer("phase_map", this->phase_map);

    this->alloc();
}

void Wavetable::alloc()
{
    this->current_phase.resize(this->num_output_channels_allocated);
}

void Wavetable::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->sync, channel, frame))
            {
                this->current_phase[channel] = 0.0;
            }

            float frequency = this->frequency->out[channel][frame];

            // TODO Create wavetable buffer
            float index = this->current_phase[channel] + this->phase->out[channel][frame];
            index = fmod(index, 1);
            while (index < 0)
            {
                index += 1;
            }
            if (this->phase_map)
            {
                index = this->phase_map->get_frame(index * this->phase_map->get_num_frames());
            }

            float rv = this->buffer->get_frame(index * this->buffer->get_num_frames());

            out[channel][frame] = rv;

            this->current_phase[channel] += (frequency / this->graph->get_sample_rate());
            while (this->current_phase[channel] >= 1.0)
                this->current_phase[channel] -= 1.0;
        }
    }
}

Wavetable2D::Wavetable2D(BufferRef2D buffer, NodeRef frequency, NodeRef crossfade, NodeRef sync)
    : buffer(buffer), frequency(frequency), crossfade(crossfade), sync(sync)
{
    this->name = "wavetable2d";

    memset(this->phase, 0, sizeof(float) * SIGNALFLOW_MAX_CHANNELS);

    this->create_input("frequency", this->frequency);
    this->create_input("crossfade", this->crossfade);
    this->create_input("sync", this->sync);

    // Named Buffer inputs don't yet work for Buffer2Ds :-(
    // this->create_buffer("buffer", this->buffer);
}

void Wavetable2D::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float frequency = this->frequency->out[channel][frame];
            int index = this->phase[channel] * this->buffer->get_num_frames();
            float rv = this->buffer->get2D(index, this->crossfade->out[0][frame]);

            out[channel][frame] = rv;

            this->phase[channel] += (frequency / this->graph->get_sample_rate());
            while (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
