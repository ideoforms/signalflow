#import "../constants.h"
#import "../unit.h"

namespace signum::env
{

class ASR : public Unit
{
	public:
		ASR(float attack, float sustain, float release, UnitRef clock = nullptr);

		float attack;
		float sustain;
		float release;
		float phase;
		float clock_last;

		UnitRef clock = nullptr;

		virtual void trigger();
		virtual sample next();
};

}
