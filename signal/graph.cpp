#include "graph.h"
#include "node.h"
#include "core.h"
#include "synth.h"

#include "io/output/abstract.h"
#include "io/output/soundio.h"
#include "io/output/ios.h"

#include <unistd.h>

namespace libsignal
{
	Graph::Graph()
	{
		signal_init();
        
		AudioOut *audioout = new AudioOut(this);
		this->output = audioout;
		this->sample_rate = audioout->sample_rate;
		this->node_count = 0;
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
			// signal_warn("Node %s has no registered params", node->name.c_str());
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
				if (param_node->num_output_channels < node->num_input_channels && !node->no_input_automix)
				{
					signal_debug("Upmixing %s (%s wants %d channels, %s only produces %d)", param_node->name.c_str(),
						node->name.c_str(), node->num_input_channels, param_node->name.c_str(), param_node->num_output_channels);

					/*------------------------------------------------------------------------
					 * If we generate 2 channels but have 6 channels demanded, repeat
					 * them: [ 0, 1, 0, 1, 0, 1 ]
					 *-----------------------------------------------------------------------*/
					for (int out_channel_index = param_node->num_output_channels;
					         out_channel_index < node->num_input_channels;
					         out_channel_index ++)
					{
						int in_channel_index = out_channel_index % param_node->num_output_channels;
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
		this->processed_nodes.clear();
		this->pull_input(this->output, num_frames);
		this->node_count = this->processed_nodes.size();
		signal_debug("Graph: pull %d frames, %d nodes", num_frames, this->node_count);
	}

	void Graph::process(const NodeRef &root, int num_frames, int block_size)
	{
		/*------------------------------------------------------------------------
		 * Updating the routing from the tip. 
		 * This normally happens when the root is connected to the graph's
		 * audio out. Can this be improved?
		 *-----------------------------------------------------------------------*/
		root->update_channels();

		int index = 0;
		signal_debug("Graph: Performing offline process of %d frames", num_frames);
		while (index < (num_frames - block_size))
		{
			this->processed_nodes.clear();
			this->pull_input(root, block_size);
			index += block_size;
		}

		/*------------------------------------------------------------------------
		 * Process remaining samples.
		 *-----------------------------------------------------------------------*/
		printf("Processed %d frames, total %d\n", index, num_frames);
		if (index < num_frames)
		{
			signal_debug("Graph: Processing remaining %d samples", num_frames - index);
			this->processed_nodes.clear();
			this->pull_input(root, num_frames - index);
		}

		// TODO Remove this?
		this->processed_nodes.clear();
		signal_debug("Graph: Offline process completed");
	}

	NodeRef Graph::add_node(Node *node)
	{
		return std::shared_ptr<Node>(node);
	}

	NodeRef Graph::get_output()
	{
		return this->output;
	}

	void Graph::add_output(SynthRef synth)
	{
		this->output->add_input(synth->output);
	}

	void Graph::add_output(NodeRef node)
	{
		this->output->add_input(node);
	}
}
