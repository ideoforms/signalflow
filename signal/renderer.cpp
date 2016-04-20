#include "renderer.h"
#include "gen/constant.h"

namespace libsignal
{

GraphRenderer::GraphRenderer()
{
}

void GraphRenderer::render_node(NodeRef node)
{
	/*------------------------------------------------------------------------
	 * Don't render nodes repeatedly
	 *-----------------------------------------------------------------------*/
	if (this->rendered_nodes.find(node.get()) != this->rendered_nodes.end())
		return;
	this->rendered_nodes.insert(node.get());

	if (node->name == "constant")
	{
		/*------------------------------------------------------------------------
		 * Render constant numeric values in a ring
		 *-----------------------------------------------------------------------*/
		nodestream << "\"" << (void const *) node.get() << "\" [shape=circle, label = \"";
		nodestream << ((Constant *) node.get())->value;
		nodestream << "\"]; ";
	}
	else
	{
		/*------------------------------------------------------------------------
		 * Render Node names in a box
		 *-----------------------------------------------------------------------*/
		nodestream << "\"" << (void const *) node.get() << "\" [fontname=\"helvetica-bold\", label = \"";
		nodestream << node->name;
		nodestream << "\"]; ";
	}

	/*------------------------------------------------------------------------
	 * Collate edges, titled based on their node input
	 *-----------------------------------------------------------------------*/
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
	/*------------------------------------------------------------------------
	 * Recurse and render the complete output
	 *-----------------------------------------------------------------------*/
	this->rendered_nodes.clear();
	this->render_node(graph->output);
	std::cout << "digraph { splines=ortho; graph [pad=1, ranksep=0.5, nodesep=0.5]; node [ fontname = helvetica, fontsize = 11, shape = box  ]; edge [ fontname = helvetica, fontsize = 9 ]; " << nodestream.str() << edgestream.str() << "} " << std::endl;
}

} /* namespace libsignal */
