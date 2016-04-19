#pragma once

#include "graph.h"

namespace libsignal
{
	class GraphRenderer
	{
		public:
			GraphRenderer();
			void render(GraphRef graph);
			void render_node(NodeRef node);

			std::stringstream nodestream;
			std::stringstream edgestream;
			std::set <Node *> rendered_nodes;
	};
}
