#pragma once

/**-------------------------------------------------------------------------
 * @file fftbuffer.h
 * @brief Stores one or more spectra of FFT mag/phase pairs.
 *
 *-----------------------------------------------------------------------*/

#include "signalflow/core/constants.h"
#include "signalflow/core/util.h"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace signalflow
{

template <class T>
class FFTBufferRefTemplate;
class FFTBuffer;
typedef FFTBufferRefTemplate<FFTBuffer> FFTBufferRef;

class FFTBuffer
{
public:
    /**------------------------------------------------------------------------
     * Construct an FFT buffer of specified dimensions.
     * The contents are initialised to zero.
     *
     *------------------------------------------------------------------------*/
    FFTBuffer(int num_frames, int fft_size, int hop_size);

    /**------------------------------------------------------------------------
     * Load the contents of the spectra file `filename` into a new buffer.
     *
     *------------------------------------------------------------------------*/
    FFTBuffer(std::string filename, int fft_size, int hop_size);

    /**------------------------------------------------------------------------
      * Destroy the buffer.
      *
      *------------------------------------------------------------------------*/
    virtual ~FFTBuffer();

    /**------------------------------------------------------------------------
     * Resize the FFT buffer.
     *
     * @param num_frames The number of FFT frames to allocate.
     *
     *------------------------------------------------------------------------*/
    void resize(int num_frames);

    /**------------------------------------------------------------------------
     * Get the magnitude array corresponding to the given frame.
     *
     * @param frame The frame index, between [0, num_frames].
     * @returns A sample value, between [-1, 1].
     *
     *------------------------------------------------------------------------*/
    sample *get_frame(int frame);
    sample *get_magnitudes(int frame);
    sample *get_phases(int frame);

    /**------------------------------------------------------------------------
     * Get the buffer's audio sample rate.
     *
     * @returns The sample rate, in Hz.
     *
     *------------------------------------------------------------------------*/
    float get_sample_rate();

    /**------------------------------------------------------------------------
     * Set the buffer's audio sample rate.
     *
     * @param sample_rate The sample rate
     *
     *------------------------------------------------------------------------*/
    void set_sample_rate(float sample_rate);

    /**------------------------------------------------------------------------
     * Get the number of spectrum frames in the buffer.
     *
     * @returns The number of frames.
     *
     *------------------------------------------------------------------------*/
    unsigned long get_num_frames();

    /**------------------------------------------------------------------------
     * Get the duration of the buffer, based on the hop_size.
     *
     * @returns The duration, in seconds.
     *
     *------------------------------------------------------------------------*/
    float get_duration();

    /**------------------------------------------------------------------------
     * Get the buffer's FFT size.
     *
     * @returns The FFT size.
     *
     *------------------------------------------------------------------------*/
    unsigned int get_fft_size();

    /**------------------------------------------------------------------------
     * Get the buffer's hop size.
     *
     * @returns The hop size.
     *
     *------------------------------------------------------------------------*/
    unsigned int get_hop_size();

    /**------------------------------------------------------------------------
     * Get the number of FFT bins in the buffer, which is equal to
     * (fft_size / 2) + 1.
     *
     * @returns The number of bins.
     *
     *------------------------------------------------------------------------*/
    unsigned int get_num_bins();

    /**------------------------------------------------------------------------
     * Get the filename that the buffer was loaded from / saved to, if set.
     *
     * @returns The filename, or an empty string.
     *
     *------------------------------------------------------------------------*/
    std::string get_filename();

    sample **data = nullptr;

protected:
    unsigned long get_total_num_values();

    std::string filename;
    float sample_rate;
    unsigned int num_frames;
    unsigned int fft_size;
    unsigned int num_bins;
    unsigned int hop_size;
    float duration;
};

template <class T>
class FFTBufferRefTemplate : public std::shared_ptr<T>
{
public:
    using std::shared_ptr<T>::shared_ptr;

    FFTBufferRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    FFTBufferRefTemplate(T *ptr)
        : std::shared_ptr<T>(ptr) {}
};

}
