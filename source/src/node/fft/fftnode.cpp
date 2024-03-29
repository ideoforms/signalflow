#include "signalflow/node/fft/fftnode.h"
#include <vector>

namespace signalflow
{

FFTNode::FFTNode(int fft_size, int hop_size, int window_size, bool do_window)
    : Node(), fft_size(fft_size), hop_size(hop_size), window_size(window_size ? window_size : fft_size), do_window(do_window)
{
    /*------------------------------------------------------------------------
     * Extra bin to store Nyquist frequency.
     *-----------------------------------------------------------------------*/
    this->num_bins = fft_size / 2 + 1;
    this->num_hops = 0;
    this->set_channels(1, 1);

    /*------------------------------------------------------------------------
     * Extra 2 floats to store complex Nyquist coefficients.
     *-----------------------------------------------------------------------*/
    this->output_buffer_length = SIGNALFLOW_MAX_FFT_SIZE + 2;
    this->resize_output_buffers(SIGNALFLOW_MAX_CHANNELS);

    this->magnitudes = new float *[SIGNALFLOW_MAX_CHANNELS]();
    for (int i = 0; i < SIGNALFLOW_MAX_CHANNELS; i++)
    {
        this->magnitudes[i] = this->out[i];
    }

    this->phases = new float *[SIGNALFLOW_MAX_CHANNELS]();
    for (int i = 0; i < SIGNALFLOW_MAX_CHANNELS; i++)
    {
        this->phases[i] = this->out[i] + this->num_bins;
    }
}

FFTNode::~FFTNode()
{
    delete this->magnitudes;
    delete this->phases;
}

std::vector<float> FFTNode::get_magnitudes()
{
    return std::vector<float>(this->magnitudes[0],
                              this->magnitudes[0] + this->num_bins);
}

std::vector<float> FFTNode::get_phases()
{
    return std::vector<float>(this->phases[0],
                              this->phases[0] + this->num_bins);
}

FFTOpNode::FFTOpNode(NodeRef input)
    : FFTNode(input ? ((FFTNode *) input.get())->fft_size : SIGNALFLOW_DEFAULT_FFT_SIZE,
              input ? ((FFTNode *) input.get())->hop_size : SIGNALFLOW_DEFAULT_FFT_HOP_SIZE,
              input ? ((FFTNode *) input.get())->window_size : SIGNALFLOW_DEFAULT_FFT_WINDOW_SIZE,
              input ? ((FFTNode *) input.get())->do_window : SIGNALFLOW_DEFAULT_FFT_DO_WINDOW)
    , input(input)
{
    this->create_input("input", this->input);

    if (dynamic_cast<FFTNode *>(input.get()) == nullptr)
    {
        throw std::runtime_error("Input to FFT operation nodes must be an FFT node");
    }
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
