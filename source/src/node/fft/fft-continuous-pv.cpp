#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"
#include "signalflow/node/fft/fft-continuous-pv.h"

namespace signalflow
{

FFTContinuousPhaseVocoder::FFTContinuousPhaseVocoder(NodeRef input, float rate)
    : FFTNode(input ? ((FFTNode *) input.get())->fft_size : SIGNALFLOW_DEFAULT_FFT_SIZE,
              input ? ((FFTNode *) input.get())->hop_size : SIGNALFLOW_DEFAULT_FFT_HOP_SIZE,
              input ? ((FFTNode *) input.get())->window_size : SIGNALFLOW_DEFAULT_FFT_WINDOW_SIZE,
              input ? ((FFTNode *) input.get())->do_window : SIGNALFLOW_DEFAULT_FFT_DO_WINDOW)
    , input(input)
    , rate(rate)
{
    /*--------------------------------------------------------------------------------
     * Why does this not just subclass FFTOpNode?
     * Because if `input` is registered as a formal input, its content will
     * be pulled automatically by the Graph. In this rare case, we don't want this
     * to happen - the PV should process its input independently.
     *--------------------------------------------------------------------------------*/
    this->name = "fft-continuous-pv";

    this->magnitude_buffer = new sample[this->num_bins]();
    this->phase_buffer = new sample[this->num_bins]();
    this->phase_deriv = new sample[this->num_bins]();
    this->prefilled_fft_buffer = false;
}

void FFTContinuousPhaseVocoder::trigger(std::string name, float value)
{
    //   this->prefilled_fft_buffer = false;
}

void FFTContinuousPhaseVocoder::process(Buffer &out, int num_frames)
{
    FFTNode *fftin = (FFTNode *) this->input.get();
    this->num_hops = 1;

    if (!prefilled_fft_buffer)
    {
        for (int i = 0; i < fft_size / this->graph->get_output_buffer_size(); i++)
        {
            this->graph->reset_subgraph(this->input);
            this->graph->render_subgraph(this->input, this->graph->get_output_buffer_size());
        }
        this->prefilled_fft_buffer = true;
    }

    this->graph->reset_subgraph(this->input);
    this->graph->render_subgraph(this->input, hop_size);

    for (int frame = 0; frame < this->num_bins; frame++)
    {
        this->phase_buffer[frame] = random_uniform(-M_PI, M_PI);
    }

    memcpy(this->phase_deriv, fftin->phases[0], this->num_bins * sizeof(sample));
    memcpy(this->magnitude_buffer, fftin->magnitudes[0], this->num_bins * sizeof(sample));

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Rather than num_frames here, we need to iterate over fft_size frames
         *  - as each block contains a whole fft of samples.
         *-----------------------------------------------------------------------*/
        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                out[hop][frame] = magnitude_buffer[frame];
            }
            else
            {
                /*------------------------------------------------------------------------
                 * Copy magnitudes in to out;
                 * for phases, increase by phase_deriv per hop
                 *-----------------------------------------------------------------------*/
                int phase_index = frame - num_bins;
                phase_buffer[phase_index] = phase_buffer[phase_index] + phase_deriv[phase_index];
                if (phase_buffer[phase_index] >= M_PI)
                    phase_buffer[phase_index] -= 2.0 * M_PI;
                out[hop][frame] = phase_buffer[phase_index];
            }
        }
    }
}

}
