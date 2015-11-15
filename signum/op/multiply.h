#include "../constants.h"

#include "../unit.h"
#include "../gen/constant.h"

namespace signum::op
{

class Multiply : public BinaryOpUnit
{

public:

	Multiply(UnitRef a, UnitRef b) : BinaryOpUnit(a, b) {};
	Multiply(UnitRef a, sample b) : BinaryOpUnit(a, b) {};
	// Multiply(UnitRef a, sample b) : inputA(a), inputB(new gen::Constant(b)) {};
	// Multiply(sample a, UnitRef b) : inputA(new gen::Constant(a)), inputB(b) {};

	virtual sample next()
	{
		return this->inputs[0]->next() * this->inputs[1]->next();
	}
};

}
