#import "../constants.h"
#import "../unit.h"

namespace signum::env
{

class ASR : public Unit
{
	public:
		ASR(float attack, float sustain, float release);

		float attack;
		float sustain;
		float release;
		float phase;

		virtual void trigger();
		virtual void next(Buffer &buffer, int count);
};

}
