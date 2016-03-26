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
			NodeRef add_node(Node *node);

			NodeRef input = nullptr;
			NodeRef output = nullptr;

			float sample_rate;

		private: 

			std::set<Node *> processed_nodes;
	};

    class GraphRef : public std::shared_ptr<Graph>
    {
        public:
            using std::shared_ptr<Graph>::shared_ptr;

            GraphRef() : std::shared_ptr<Graph>(nullptr) { }
            GraphRef(Graph *ptr) : std::shared_ptr<Graph>(ptr) { }
    };
}
