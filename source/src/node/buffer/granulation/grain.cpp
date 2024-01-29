#include "signalflow/node/buffer/granulation/grain.h"

Grain::Grain(BufferRef buffer, int phase, int length_samples, float rate, float pan, bool wrap)
    : buffer(buffer), phase(phase), length_samples(length_samples), rate(rate), pan(pan), wrap(wrap)
{
    this->samples_processed = 0;
    if (rate < 0)
    {
        /*------------------------------------------------------------------------
         * If playback is reversed, begin at the end of the grain.
         *-----------------------------------------------------------------------*/
        this->phase += length_samples;
    }
}

void Grain::step()
{
    /*------------------------------------------------------------------------
     * Note that the `rate` attribute of a grain always assumes that
     * any buffer-specific rate_scale_factor has already been applied.
     *-----------------------------------------------------------------------*/
    this->phase += this->rate;

    if (this->wrap)
    {
        while (this->phase >= this->buffer->get_num_frames())
            this->phase -= this->buffer->get_num_frames();
        while (this->phase < 0)
            this->phase += this->buffer->get_num_frames();
    }

    this->samples_processed += 1;
}

double Grain::get_progress()
{
    /*------------------------------------------------------------------------
     * Subtract 1 to ensure that the progress output is 0..1 inclusive
     *-----------------------------------------------------------------------*/
    return (double) this->samples_processed / (this->length_samples - 1);
}

bool Grain::is_finished()
{
    if (this->samples_processed >= this->length_samples)
    {
        return true;
    }
    else if (!wrap)
    {
        return (this->phase >= this->buffer->get_num_frames() || this->phase < 0);
    }
    else
    {
        return false;
    }
}