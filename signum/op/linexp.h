#include "../constants.h"

#include "../unit.h"
#include "../gen/constant.h"

namespace signum::op
{

class LinExp : public UnaryOpUnit
{

public:

	LinExp(UnitRef input, UnitRef a, UnitRef b, UnitRef c, UnitRef d) : UnaryOpUnit(input), a(a), b(b), c(c), d(d)
	{
		this->name = "linexp";
	}

	virtual void next(sample **out, int num_frames)
	{
		UnitRef input = this->inputs[0];
		for (int frame = 0; frame < num_frames; frame++)
		{
			for (int channel = 0; channel < this->channels_out; channel++)
			{
				float norm = (input->out[channel][frame] - a->out[channel][frame]) / (b->out[channel][frame] - a->out[channel][frame]);
				out[channel][frame] = powf(d->out[channel][frame] / c->out[channel][frame], norm) * c->out[channel][frame];
			}
		}
	}

	UnitRef a;
	UnitRef b;
	UnitRef c;
	UnitRef d;
};

}
