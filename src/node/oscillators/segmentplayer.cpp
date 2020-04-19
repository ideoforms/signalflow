#include "signal/node/oscillators/segmentplayer.h"

#include <stdlib.h>

namespace libsignal
{

SegmentPlayer::SegmentPlayer(BufferRef buffer, PropertyRef onsets) : buffer(buffer)
{
    this->name = "segment_player";

    this->num_input_channels = 0;
    this->num_output_channels = buffer->num_channels;

    this->min_input_channels = this->max_input_channels = 0;
    this->min_output_channels = this->max_output_channels = this->num_output_channels;

    this->phase = 0.0;

    this->set_property("onsets", { });
    this->trigger();
}

void SegmentPlayer::process(sample **out, int num_frames)
{
    sample s;

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if ((int) this->phase < buffer->num_frames)
            {
                s = this->buffer->data[channel][(int) this->phase];
            }
            else
            {
                s = 0.0;
            }

            out[channel][frame] = s;
        }

        this->phase += 1.0;
    }
}

void SegmentPlayer::trigger(std::string name, float value)
{
    PropertyRef onsetsref = this->get_property("onsets");
    if (onsetsref)
    {
        std::vector <float> onsets = onsetsref->float_array_value();
        if (onsets.size() > 0)
        {
            int index = random_integer(0, onsets.size());
            this->phase = onsets[index];
            printf("set phase: %f\n", this->phase);
        }
    }
}

}
