#include "../unit.h"
#include "../constants.h"
#include "../buffer.h"

namespace signum::gen
{
	class Sampler : public Unit
	{
		public:
			Sampler(Buffer *buffer, UnitRef rate = 1.0, bool loop = false);

			Buffer *buffer;

			UnitRef rate;
			float phase;
			bool loop;

			virtual void trigger();
			virtual sample next();
	};
}
