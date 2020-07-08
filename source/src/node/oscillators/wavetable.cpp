#include "signal/core/graph.h"
#include "signal/node/oscillators/wavetable.h"

namespace libsignal
{

Wavetable::Wavetable(BufferRef table, NodeRef frequency, NodeRef sync)
    : table(table), frequency(frequency), sync(sync)
{
    this->name = "wavetable";

    memset(this->phase, 0, sizeof(float) * SIGNAL_MAX_CHANNELS);

    this->add_input("frequency", this->frequency);
    this->add_input("sync", this->sync);
}

void Wavetable::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            printf("check trig\n");
            if (SIGNAL_CHECK_CHANNEL_TRIGGER(this->sync, channel, frame))
            {
                this->phase[channel] = 0.0;
            }

            printf("get freq\n");
            float frequency = this->frequency->out[channel][frame];
            printf("query table\n");
            int index = this->phase[channel] * this->table->num_frames;
            float rv = this->table->get(index);

            out[channel][frame] = rv;

            this->phase[channel] += (frequency / this->graph->get_sample_rate());
            while (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

Wavetable2D::Wavetable2D(BufferRef2D buffer, NodeRef frequency, NodeRef crossfade, NodeRef sync)
    : buffer(buffer), frequency(frequency), crossfade(crossfade), sync(sync)
{
    this->name = "wavetable2d";

    memset(this->phase, 0, sizeof(float) * SIGNAL_MAX_CHANNELS);

    this->add_input("frequency", this->frequency);
    this->add_input("crossfade", this->crossfade);
    this->add_input("sync", this->sync);
}

void Wavetable2D::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float frequency = this->frequency->out[0][frame];
            int index = this->phase[channel] * this->buffer->num_frames;
            float rv = this->buffer->get2D(index, this->crossfade->out[0][frame]);

            out[channel][frame] = rv;

            this->phase[channel] += (frequency / this->graph->get_sample_rate());
            while (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
