#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"
#include "signalflow/node/buffer/grainsegments.h"

namespace signalflow
{

GrainSegments::GrainSegments(BufferRef buffer, NodeRef clock, NodeRef target, PropertyRef offsets, PropertyRef values, PropertyRef durations)
    : buffer(buffer), clock(clock), target(target), offsets(offsets), values(values), durations(durations)
{
    this->name = "grain-segments";

    this->add_input("clock", this->clock);
    this->add_input("target", this->target);

    // add properties
    this->set_property("offsets", this->offsets);
    this->set_property("values", this->values);
    this->set_property("durations", this->durations);

    this->add_buffer(" buffer", buffer);

    // this->envelope = new EnvelopeBufferHanning();
    this->envelope = new EnvelopeBuffer();
    this->add_buffer("envelope", this->envelope);

    this->set_channels(1, 2);

    this->clock_last = 0.0;
}

void GrainSegments::process(sample **out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process .
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    // printf("sample_rate now = %f\n", this->graph->get_sample_rate());
    sample frequency = this->target->out[0][0];
    frequency = signal_midi_note_to_frequency(roundf(signal_frequency_to_midi_note(frequency)));
    std::vector<float> offsets = this->get_property("offsets")->float_array_value();
    std::vector<float> values = this->get_property("values")->float_array_value();
    std::vector<float> durations = this->get_property("durations")->float_array_value();

    std::vector<int> indices;

    for (size_t i = 0; i < values.size(); i++)
    {
        if (frequency == values[i])
        {
            indices.push_back(i);
        }
    }

    for (int frame = 0; frame < num_frames; frame++)
    {
        sample clock_value = this->clock->out[0][frame];
        sample pan = 0.5;

        if (clock_value > clock_last)
        {
            /*------------------------------------------------------------------------
             * Trigger grain
             *-----------------------------------------------------------------------*/
            if (indices.size() > 0)
            {
                int indexindex = random_integer(0, indices.size());
                int index = indices[indexindex];
                float offset = offsets[index];     // * this->graph->get_sample_rate();
                float duration = durations[index]; // * this->graph->get_sample_rate();
                Grain *grain = new Grain(buffer, offset, duration, 1.0, pan);
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
                int buffer_index = grain->sample_start + grain->samples_done;
                while (buffer_index > this->buffer->get_num_frames())
                    buffer_index -= this->buffer->get_num_frames();
                sample s = this->buffer->data[0][(int) buffer_index];

                /*------------------------------------------------------------------------
                 * Apply grain envelope.
                 *-----------------------------------------------------------------------*/
                float env_phase = (float) (grain->samples_done) / grain->sample_length;
                float amp = this->envelope->get(env_phase);

                grain->samples_done += grain->rate;

                /*------------------------------------------------------------------------
                 * Calculate pan.
                 * TODO: Handle >2 channels
                 *-----------------------------------------------------------------------*/
                float rv = s * amp;
                out[0][frame] += rv * (1.0 - grain->pan);
                out[1][frame] += rv * (grain->pan);

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

}
