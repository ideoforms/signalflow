#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/wavetable.h"

namespace signalflow
{

Wavetable::Wavetable(BufferRef buffer, NodeRef frequency, NodeRef phase_offset, NodeRef sync, BufferRef phase_map)
    : buffer(buffer), frequency(frequency), phase_offset(phase_offset), sync(sync), phase_map(phase_map)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "wavetable";

    this->create_input("frequency", this->frequency);
    this->create_input("phase_offset", this->phase_offset);
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

    /*--------------------------------------------------------------------------------
     * Take local copies of values that are constant across the block, and keep
     * phase in a local variable during the loop, to avoid a load and store per
     * sample (see SineOscillator).
     *--------------------------------------------------------------------------------*/
    Buffer *buffer = this->buffer.get();
    Buffer *phase_map = this->phase_map.get();
    Node *sync = this->sync.get();
    unsigned long buffer_num_frames = buffer->get_num_frames();
    unsigned long phase_map_num_frames = phase_map ? phase_map->get_num_frames() : 0;
    float sample_rate = this->graph->get_sample_rate();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        float phase = this->current_phase[channel];
        sample *out_channel = out[channel];
        sample *frequency_channel = this->frequency->out[channel];
        sample *phase_offset_channel = this->phase_offset->out[channel];

        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(sync, channel, frame))
            {
                phase = 0.0;
            }

            float frequency = frequency_channel[frame];

            // TODO Create wavetable buffer
            float index = phase + phase_offset_channel[frame];
            index = fmodf(index, 1);
            while (index < 0)
            {
                index += 1;
            }
            if (phase_map)
            {
                index = phase_map->get_frame(0, index * phase_map_num_frames);
            }

            float rv = buffer->get_frame(0, index * buffer_num_frames);

            out_channel[frame] = rv;

            phase += (frequency / sample_rate);
            while (phase >= 1.0)
                phase -= 1.0;
        }
        this->current_phase[channel] = phase;
    }
}

Wavetable2D::Wavetable2D(BufferRef2D buffer, NodeRef frequency, NodeRef crossfade, NodeRef phase_offset, NodeRef sync)
    : buffer(buffer), frequency(frequency), crossfade(crossfade), phase_offset(phase_offset), sync(sync)
{
    this->name = "wavetable2d";

    this->create_input("frequency", this->frequency);
    this->create_input("crossfade", this->crossfade);
    this->create_input("phase_offset", this->phase_offset);
    this->create_input("sync", this->sync);

    // Named Buffer inputs don't yet work for Buffer2Ds :-(
    // this->create_buffer("buffer", this->buffer);

    this->alloc();
}

void Wavetable2D::alloc()
{
    this->current_phase.resize(this->num_output_channels_allocated);
}

void Wavetable2D::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float frequency = this->frequency->out[channel][frame];

            float current_phase = this->current_phase[channel] + this->phase_offset->out[channel][frame];
            current_phase = fmod(current_phase, 1);
            while (current_phase < 0)
            {
                current_phase += 1;
            }

            float index = current_phase * this->buffer->get_num_frames();
            float rv = this->buffer->get2D(index, this->crossfade->out[0][frame]);

            out[channel][frame] = rv;

            this->current_phase[channel] += (frequency / this->graph->get_sample_rate());
            while (this->current_phase[channel] >= 1.0)
                this->current_phase[channel] -= 1.0;
        }
    }
}

}
