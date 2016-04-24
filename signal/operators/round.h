#include "../constants.h"

#include "../node.h"
#include "../util.h"
#include "../oscillators/constant.h"

namespace libsignal
{

class RoundToScale : public UnaryOpNode
{

public:

	RoundToScale(NodeRef a) : UnaryOpNode(a)
	{
		this->name = "round-to-scale";
	}

	virtual void process(sample **out, int num_frames)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			for (int channel = 0; channel < this->num_output_channels; channel++)
			{
				float value = this->input->out[channel][frame];
				float midi = freq_to_midi(value);
				float midi_rounded = roundf(midi);
				float freq = midi_to_freq(midi_rounded);
				out[channel][frame] = freq;
			}
		}
	}
};

}
