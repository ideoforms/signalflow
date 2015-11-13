#include "constant.h"

namespace signum::gen
{

Constant::Constant(sample value) : Unit()
{
	this->value = value;
}

sample Constant::next()
{
	return this->value;
}

}
