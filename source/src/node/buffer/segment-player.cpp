#include "signalflow/core/random.h"
#include "signalflow/node/buffer/segment-player.h"

#include <stdlib.h>

namespace signalflow
{

SegmentPlayer::SegmentPlayer(BufferRef buffer, PropertyRef onsets)
    : buffer(buffer)
{
    this->name = "segment-player";

    this->num_input_channels = 1;
    this->num_output_channels = buffer->get_num_channels();
    this->matches_input_channels = false;

    this->add_buffer("buffer", buffer);

    this->phase = 0.0;

    this->set_property("onsets", {});
    this->trigger();
}

void SegmentPlayer::process(sample **out, int num_frames)
{
    sample s;

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if ((int) this->phase < buffer->get_num_frames())
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
        std::vector<float> onsets = onsetsref->float_array_value();
        if (onsets.size() > 0)
        {
            int index = random_integer(0, onsets.size());
            this->phase = onsets[index];
        }
    }
}

}
