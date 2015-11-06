#include "../constants.h"
#include "../unit.h"

namespace signum::op
{

class Multiply : public Unit
{

public:

	Multiply(Unit &a, Unit &b);
	Multiply(Unit &a, sample b);
	Multiply(sample a, Unit &b);

	Unit *a;
	Unit *b;

	virtual void next(int count);
};

}
