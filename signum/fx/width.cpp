#include "width.h" 

namespace signum::fx
{

void Width::next(sample *in, sample *out)
{
	sample left = in[0];
	sample right = in[1];

	sample width = this->width->next();
	left = (1 - width) * left + width * right;
	right = (1 - width) * right + width * left;

	out[0] = left;
	out[1] = right;
}


}
