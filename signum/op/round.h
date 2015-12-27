#include "../constants.h"

#include "../unit.h"
#include "../util.h"
#include "../gen/constant.h"

namespace signum
{

class RoundToScale : public UnaryOpUnit
{

public:

	RoundToScale(UnitRef a) : UnaryOpUnit(a)
	{
		this->name = "round-to-scale";
	}

	virtual void next(sample **out, int num_frames)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			for (int channel = 0; channel < this->channels_out; channel++)
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
