#include "signal/node/oscillators/wavetable.h"
#include "signal/core/graph.h"

namespace libsignal
{

Wavetable::Wavetable(BufferRef table, NodeRef frequency) :
    table(table), frequency(frequency)
{
    this->name = "wavetable";
    this->phase = 0.0;

    this->add_input("frequency", this->frequency);
}

void Wavetable::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float frequency = this->frequency->out[0][frame];
        int index = this->phase * this->table->num_frames;
        float rv = this->table->get(index);

        this->out[0][frame] = rv;

        this->phase += (frequency / this->graph->sample_rate);
        while (this->phase >= 1.0)
            this->phase -= 1.0;
    }
}

Wavetable2D::Wavetable2D(BufferRef2D buffer, NodeRef frequency, NodeRef crossfade) :
        buffer(buffer), frequency(frequency), crossfade(crossfade)
{
    this->name = "wavetable2D";
    this->phase = 0.0;

    this->add_input("frequency", this->frequency);
    this->add_input("crossfade", this->crossfade);
}

void Wavetable2D::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float frequency = this->frequency->out[0][frame];
        int index = this->phase * this->buffer->num_frames;
        float rv = this->buffer->get2D(index, this->crossfade->out[0][frame]);

        this->out[0][frame] = rv;

        this->phase += (frequency / this->graph->sample_rate);
        while (this->phase >= 1.0)
            this->phase -= 1.0;
    }
}

}
