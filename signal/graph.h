#pragma once 

#include "node.h"

namespace libsignal
{
	class AudioOut_Abstract;

	class Graph
	{
		public:

			Graph();

			void run();
			void pull_input(const NodeRef & unit, int num_frames);
			void pull_input(int num_frames);
			NodeRef addNode(Node *unit);

			NodeRef input = nullptr;
			NodeRef output = nullptr;

			float sample_rate;
	};
}
