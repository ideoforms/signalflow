#include "node.h"

#include <unistd.h>
#include <thread>

namespace libsignal
{

class NodeMonitor
{
	public:
	NodeMonitor(Node *node, std::string label, float frequency)
	{
		this->node = node;
		this->frequency = frequency;

		if (label == "")
			this->label = this->node->name;
		else
			this->label = label;
	}

	void start()
	{
		this->running = true;
		this->thread = std::shared_ptr<std::thread>(new std::thread(&NodeMonitor::run_thread, this));
	}

	void run_thread()
	{
		float sleep_time = 1000000.0 * (1.0 / this->frequency);
		while (this->running)
		{
			fprintf(stderr, "%s: %.5f\n", this->label.c_str(), this->node->out[0][0]);
			usleep(sleep_time);
		}
	}

	void stop()
	{
		this->running = false;
	}

	Node *node;
	float frequency;
	std::string label;
	bool running;
	std::shared_ptr<std::thread> thread;
};

}
