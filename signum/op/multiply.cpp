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

sample Multiply::next()
{
	return this->a->next() * this->b->next();
}

}
