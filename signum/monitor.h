#include "unit.h"

#include <unistd.h>
#include <thread>

namespace signum
{

class UnitMonitor
{
	public:
	UnitMonitor(Unit *unit, std::string label, float frequency)
	{
		this->unit = unit;
		this->frequency = frequency;

		if (label == "")
			this->label = this->unit->name;
		else
			this->label = label;
	}

	void start()
	{
		this->running = true;
		this->thread = std::shared_ptr<std::thread>(new std::thread(&UnitMonitor::run_thread, this));
	}

	void run_thread()
	{
		float sleep_time = 1000000.0 * (1.0 / this->frequency);
		while (this->running)
		{
			fprintf(stderr, "%s: %.5f\n", this->label.c_str(), this->unit->out[0][0]);
			usleep(sleep_time);
		}
	}

	void stop()
	{
		this->running = false;
	}

	Unit *unit;
	float frequency;
	std::string label;
	bool running;
	std::shared_ptr<std::thread> thread;
};

}
