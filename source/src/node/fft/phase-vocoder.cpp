#include "signal/node/fft/phase-vocoder.h"

namespace libsignal
{

FFTPhaseVocoder::FFTPhaseVocoder(NodeRef input)
    : FFTOpNode(input)
{
    this->name = "fft-phase-vocoder";

    this->add_input("clock", this->clock);

    this->phase_buffer = new sample[this->num_bins]();
    this->phase_deriv = new sample[this->num_bins]();
    this->magnitude_buffer = new sample[this->num_bins]();

    this->frozen = false;
    this->just_frozen = false;
}

void FFTPhaseVocoder::trigger(std::string name, float value)
{
    if (name == SIGNAL_DEFAULT_TRIGGER || name == "freeze")
    {
        this->frozen = true;
        this->just_frozen = true;
    }
    else if (name == "unfreeze")
    {
        this->frozen = false;
    }
}

void FFTPhaseVocoder::process(sample **out, int num_frames)
{
    if (this->clock)
    {
        SIGNAL_PROCESS_TRIGGER_BLOCK(this->clock, num_frames, SIGNAL_DEFAULT_TRIGGER);
    }

    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Rather than num_frames here, we need to iterate over fft_size frames
         *  - as each block contains a whole fft of samples.
         *-----------------------------------------------------------------------*/
        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frozen)
            {
                if (frame < this->num_bins)
                {
                    out[hop][frame] = magnitude_buffer[frame];
                }
                else
                {
                    /*------------------------------------------------------------------------
                     * copy magnitudes in to out;
                     * for phases, increase by phase_deriv per hop
                     *-----------------------------------------------------------------------*/
                    int phase_index = frame - num_bins;
                    phase_buffer[phase_index] = phase_buffer[phase_index] + phase_deriv[phase_index];
                    if (phase_buffer[phase_index] >= M_PI)
                        phase_buffer[phase_index] -= 2.0 * M_PI;
                    out[hop][frame] = phase_buffer[phase_index];
                }
            }
            else
            {
                /*------------------------------------------------------------------------
                 * Copy in to out
                 *-----------------------------------------------------------------------*/
                out[hop][frame] = input->out[hop][frame];
            }
        }
    }

    int last_hop = this->num_hops - 1;
    if ((last_hop >= 1) && (!frozen || just_frozen))
    {
        this->just_frozen = false;
        FFTNode *fftin = (FFTNode *) input.get();
        for (int frame = 0; frame < this->num_bins; frame++)
        {
            this->phase_deriv[frame] = fftin->phases[last_hop][frame] - fftin->phases[last_hop - 1][frame];
            this->phase_buffer[frame] = fftin->phases[last_hop][frame];
            this->magnitude_buffer[frame] = fftin->magnitudes[last_hop][frame];
        }
    }
}

}
