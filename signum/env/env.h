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

		UnitRef clock = nullptr;

		virtual void trigger();
		virtual void next(int count);
};

}
