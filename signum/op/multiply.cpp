#include "multiply.h"

namespace signum::op
{

Multiply::Multiply(Unit &a, Unit &b)
{
	this->a = &a;
	this->b = &b;
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
