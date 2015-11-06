#include "constant.h"

namespace signum::gen
{

Constant::Constant(sample value) : Unit()
{
	this->value = value;
}

void Constant::next(int count)
{
	for (int i = 0; i < count; i++)
	{
		this->output->data[0][i] = value;
	}
}

}
