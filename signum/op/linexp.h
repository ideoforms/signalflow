#include "../constants.h"

#include "../unit.h"
#include "../gen/constant.h"

namespace signum::op
{

class LinExp : public UnaryOpUnit
{

public:

	LinExp(UnitRef input = 0, UnitRef a = 0, UnitRef b = 1, UnitRef c = 1, UnitRef d = 10) : UnaryOpUnit(input), a(a), b(b), c(c), d(d)
	{
		this->name = "linexp";

		this->add_param("a", this->a);
		this->add_param("b", this->b);
		this->add_param("c", this->c);
		this->add_param("d", this->d);
	}

	virtual void next(sample **out, int num_frames)
	{
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


REGISTER(LinExp, "linexp");

}

