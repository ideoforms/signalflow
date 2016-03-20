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
			void pull_input(const NodeRef &node, int num_frames);
			void pull_input(int num_frames);
			NodeRef addNode(Node *node);

			NodeRef input = nullptr;
			NodeRef output = nullptr;

			float sample_rate;
	};
}
