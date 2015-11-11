#include "unit.h"

#include <unistd.h>

namespace signum
{

	class Graph
	{
		public:
			Graph()
			{
				signum_init();
			}	
			void run()
			{
				while (true) { usleep(100000); }
			}
			UnitRef addUnit(Unit *unit)
			{
				return std::shared_ptr<Unit>(unit);
			}
	};
}
