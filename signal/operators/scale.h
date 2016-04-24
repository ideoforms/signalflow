#include "../constants.h"

#include "../node.h"
#include "../oscillators/constant.h"

namespace libsignal
{

	class LinExp : public UnaryOpNode
	{

	public:

		LinExp(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10) : UnaryOpNode(input), a(a), b(b), c(c), d(d)
		{
			this->name = "linexp";

			this->add_param("a", this->a);
			this->add_param("b", this->b);
			this->add_param("c", this->c);
			this->add_param("d", this->d);
		}

		virtual void process(sample **out, int num_frames)
		{
			for (int frame = 0; frame < num_frames; frame++)
			{
				for (int channel = 0; channel < this->num_output_channels; channel++)
				{
					float norm = (input->out[channel][frame] - a->out[channel][frame]) / (b->out[channel][frame] - a->out[channel][frame]);
					out[channel][frame] = powf(d->out[channel][frame] / c->out[channel][frame], norm) * c->out[channel][frame];
				}
			}
		}

		NodeRef a, b, c, d;
	};

	class Scale : public UnaryOpNode
	{

	public:

		Scale(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10) : UnaryOpNode(input), a(a), b(b), c(c), d(d)
		{
			this->name = "scale";

			this->add_param("a", this->a);
			this->add_param("b", this->b);
			this->add_param("c", this->c);
			this->add_param("d", this->d);
		}

		virtual void process(sample **out, int num_frames)
		{
			for (int frame = 0; frame < num_frames; frame++)
			{
				for (int channel = 0; channel < this->num_output_channels; channel++)
				{
					float norm = (input->out[channel][frame] - a->out[channel][frame]) / (b->out[channel][frame] - a->out[channel][frame]);
					out[channel][frame] = (c->out[channel][frame]) + (d->out[channel][frame] - c->out[channel][frame]) * norm;
				}
			}
		}

		NodeRef a, b, c, d;
	};

	REGISTER(LinExp, "linexp");
	REGISTER(Scale, "scale");
}

