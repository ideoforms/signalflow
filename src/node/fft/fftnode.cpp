#ifdef __APPLE__

#include "signal/node/fft/fftnode.h"

namespace libsignal
{

FFTNode::FFTNode(int fft_size)
        : Node()
{
    this->fft_size = fft_size;
    this->num_bins = fft_size / 2;
    this->num_hops = 0;

    this->magnitudes = (sample **) malloc(
            SIGNAL_MAX_CHANNELS * sizeof(float *));
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++) {
        this->magnitudes[i] = this->out[i];
    }

    this->phases = (sample **) malloc(
            SIGNAL_MAX_CHANNELS * sizeof(float *));
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++) {
        this->phases[i] = this->out[i] + this->num_bins;
    }
}

FFTNode::~FFTNode()
{
    free(this->magnitudes);
    free(this->phases);
}

FFTOpNode::FFTOpNode(NodeRef input)
        : FFTNode(input ? ((FFTNode *) input.get())->fft_size
                        : SIGNAL_DEFAULT_FFT_SIZE), input(input)
{
    this->add_input("input", this->input);
}

void FFTOpNode::set_input(std::string name, const NodeRef &node)
{
    FFTNode::set_input(name, node);
    if (name == "input")
    {
        // TODO: Update FFT size and buffers
    }
}

}

#endif