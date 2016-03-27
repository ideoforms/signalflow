#include "graph.h"
#include "node.h"
#include "core.h"

#include "io/output/abstract.h"
#include "io/output/soundio.h"
#include "io/output/ios.h"

#include <unistd.h>

namespace libsignal
{
	Graph::Graph()
	{
		signal_init();
        
		this->output = new AudioOut(this);
		this->sample_rate = 44100.0;
	}

	void Graph::run()
	{
		while (true) { usleep(100000); }
	}

	void Graph::pull_input(const NodeRef &node, int num_frames)
	{
		/*------------------------------------------------------------------------
		 * If this node has already been processed this timestep, return.
		 *-----------------------------------------------------------------------*/
		if (this->processed_nodes.find(node.get()) != processed_nodes.end())
		{
			signal_debug("Already processed node %s, skipping", node->name.c_str());
			return;
		}

		if (!(node->params.size() > 0 || node->name == "constant" || node->name == "audioout" || node->name == "audioin"))
		{
			signum_warn("Node %s has no registered params", node->name.c_str());
		}

		/*------------------------------------------------------------------------
		 * Pull our inputs before we generate our own outputs.
		 *-----------------------------------------------------------------------*/

		for (auto param : node->params)
		{
			NodeRef param_node = *(param.second);
			if (param_node)
			{
				this->pull_input(param_node, num_frames);

				/*------------------------------------------------------------------------
				 * Automatic upmix.
				 *
				 * If the input node produces less channels than demanded, automatically
				 * up-mix its output by replicating the existing channels. This allows
				 * operations between multi-channel and mono-channel inputs to work
				 * seamlessly without any additional implementation within the node
				 * itself (for example, Multiply(new Sine(440), new Pan(2, ...)))
				 *
				 * A few nodes must prevent automatic up-mixing from happening.
				 * These include Multiplex and AudioOut.
				 *-----------------------------------------------------------------------*/
				if (param_node->channels_out < node->channels_in && !node->no_input_automix)
				{
					signal_debug("Upmixing %s (%s wants %d channels, %s only produces %d)", param_node->name.c_str(),
						node->name.c_str(), node->channels_in, param_node->name.c_str(), param_node->channels_out);

					/*------------------------------------------------------------------------
					 * If we generate 2 channels but have 6 channels demanded, repeat
					 * them: [ 0, 1, 0, 1, 0, 1 ]
					 *-----------------------------------------------------------------------*/
					for (int out_channel_index = param_node->channels_out;
					         out_channel_index < node->channels_in;
					         out_channel_index ++)
					{
						int in_channel_index = out_channel_index % param_node->channels_out;
						memcpy(param_node->out[out_channel_index],
						       param_node->out[in_channel_index],
						       num_frames * sizeof(sample));
					}
				}
			}
		}
		node->next(node->out, num_frames);
		this->processed_nodes.insert(node.get());
	}

	void Graph::pull_input(int num_frames)
	{
		signal_debug("Graph: pull %d frames", num_frames);
		this->processed_nodes.clear();
		this->pull_input(this->output, num_frames);
	}

	NodeRef Graph::add_node(Node *node)
	{
		return std::shared_ptr<Node>(node);
	}
}
