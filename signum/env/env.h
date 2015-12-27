#import "../constants.h"
#import "../unit.h"

namespace signum
{

class ASR : public Unit
{
	public:
		ASR(UnitRef attack = 0.1, UnitRef sustain = 0.5, UnitRef release = 0.1, UnitRef clock = nullptr);

		UnitRef attack;
		UnitRef sustain;
		UnitRef release;
		UnitRef clock = nullptr;

		float phase;
		float clock_last;


		virtual void trigger();
		virtual void next(sample **out, int num_frames);
};

REGISTER(ASR, "env-asr");

}
