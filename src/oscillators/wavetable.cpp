#include "signal/oscillators/wavetable.h"
#include "signal/graph.h"

namespace libsignal
{

Wavetable::Wavetable(BufferRef table, NodeRef frequency, bool interpolate) :
    table(table), frequency(frequency), interpolate(interpolate)
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
        float rv = this->table->data[0][index];

        this->out[0][frame] = rv;

        this->phase += (frequency / this->graph->sample_rate);
        while (this->phase >= 1.0)
            this->phase -= 1.0;
    }
}

}
