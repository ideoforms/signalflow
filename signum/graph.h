#include "unit.h"

#include <unistd.h>
#include "io/output.h"

namespace signum
{

	class Graph
	{
		public:
			Graph()
			{
				signum_init();

				// input = new io::AudioIn();
				output = new io::AudioOut();
			}	
			void run()
			{
				while (true) { usleep(100000); }
			}
			UnitRef addUnit(Unit *unit)
			{
				return std::shared_ptr<Unit>(unit);
			}

			UnitRef input = nullptr;
			UnitRef output = nullptr;
	};
}
