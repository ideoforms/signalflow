#include "../constants.h"

#include "../unit.h"
#include "../gen/constant.h"

namespace signum::op
{

class Multiply : public Unit
{

public:

	Multiply(UnitRef a, UnitRef b) : a(a), b(b) {};
	Multiply(UnitRef a, sample b) : a(a), b(new gen::Constant(b)) {};
	Multiply(sample a, UnitRef b);

	UnitRef a;
	UnitRef b;

	virtual sample next()
	{
		return this->a->next() * this->b->next();
	}
};

}
