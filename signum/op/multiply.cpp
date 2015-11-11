#include "multiply.h"

#include "../gen/constant.h"

#include <memory>

namespace signum::op
{

Multiply::Multiply(UnitRef a, UnitRef b) : a(a), b(b)
{
}

Multiply::Multiply(UnitRef a, sample b) : a(a), b(new gen::Constant(b))
{
}

void Multiply::next(int count)
{
	this->a->next(count);
	this->b->next(count);

	for (int i = 0; i < count; i++)
	{
		this->output->data[0][i] = this->a->output->data[0][i] * this->b->output->data[0][i];
	}
}

}
