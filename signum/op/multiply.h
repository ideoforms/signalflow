#include "../constants.h"

#include "../unit.h"

namespace signum::op
{

class Multiply : public Unit
{

public:

	Multiply(UnitRef a, UnitRef b);
	Multiply(UnitRef a, sample b);
	Multiply(sample a, UnitRef b);

	UnitRef a;
	UnitRef b;

	virtual sample next();
};

}
