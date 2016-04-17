#pragma once

#include "../node.h"

namespace libsignal
{
	class FFTNode : public Node
	{
		public:

			FFTNode(int fft_size) : Node()
			{
				this->fft_size = fft_size;
				this->num_bins = fft_size / 2;
				this->num_hops = 0;

				this->magnitudes = (sample **) malloc(SIGNAL_MAX_CHANNELS * sizeof(float *));
				for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
					this->magnitudes[i] = this->out[i];

				this->phases = (sample **) malloc(SIGNAL_MAX_CHANNELS * sizeof(float *));
				for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
					this->phases[i] = this->out[i] + this->num_bins;
			}

			~FFTNode()
			{
				free(this->magnitudes);
				free(this->phases);
			}

			sample **magnitudes;
			sample **phases;
			int fft_size;
			int num_bins;
			int num_hops;
	};

	class FFTOpNode : public FFTNode
	{
		public:

			FFTOpNode(NodeRef input = nullptr) : FFTNode(input ? ((FFTNode *) input.get())->fft_size : SIGNAL_DEFAULT_FFT_SIZE), input(input)
			{
				this->add_param("input", this->input);
			}

			virtual void set_param(std::string name, const NodeRef &node)
			{
				FFTNode::set_param(name, node);
				if (name == "input")
				{
					// TODO: Update FFT size and buffers
				}
			}

			NodeRef input;
	};
}

