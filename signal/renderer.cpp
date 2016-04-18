#include "renderer.h"
#include "gen/constant.h"

#include <iostream>

namespace libsignal
{

GraphRenderer::GraphRenderer()
{
}

void GraphRenderer::render_node(NodeRef node)
{
	if (node->name == "constant")
	{
		nodestream << "\"" << (void const *) node.get() << "\" [shape=circle, label = \"";
		nodestream << ((Constant *) node.get())->value;
		nodestream << "\"]; ";
	}
	else
	{
		nodestream << "\"" << (void const *) node.get() << "\" [fontname=\"helvetica-bold\", label = \"";
		nodestream << node->name;
		nodestream << "\"]; ";
	}

	for (auto param : node->params)
	{
		NodeRef value = *(param.second);
		if (value)
		{
			this->render_node(value);
			edgestream << "\"" << (void const *) value.get() << "\" -> \"" << (void const *) node.get() << "\" [fontcolor = red, labeldistance = 2, headlabel = \"" << param.first << "\"]; ";
		}
	}
}

void GraphRenderer::render(GraphRef graph)
{
	this->render_node(graph->output);
	std::cout << "digraph { splines=ortho; graph [pad=1, ranksep=0.5, nodesep=0.5]; node [ fontname = helvetica, fontsize = 11, shape = box  ]; edge [ fontname = helvetica, fontsize = 11 ]; " << nodestream.str() << edgestream.str() << "} " << std::endl;
}

} /* namespace libsignal */
