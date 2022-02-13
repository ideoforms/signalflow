#pragma once

/**-------------------------------------------------------------------------
 * @file buffer.h
 * @brief Buffer stores one or more channels of floating-point
 *        samples.
 *
 *-----------------------------------------------------------------------*/

#include "signalflow/core/constants.h"
#include "signalflow/core/util.h"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define SIGNALFLOW_DEFAULT_ENVELOPE_BUFFER_LENGTH 2048

#define SIGNALFLOW_BUFFER_HANNING "hanning"
#define SIGNALFLOW_BUFFER_LINEAR_DECAY "linear-decay"
#define SIGNALFLOW_BUFFER_TRIANGLE "triangle"

namespace signalflow
{

template <class T>
class BufferRefTemplate;
class Buffer;
typedef BufferRefTemplate<Buffer> BufferRef;

class Buffer
{
public:
    /**------------------------------------------------------------------------
     * Construct a null buffer with no storage allocated.
     * This typically isn't useful outside of internal usage (e.g.,
     * allocating a placeholder buffer that will later be replaced).
     *
     *------------------------------------------------------------------------*/
    Buffer();

    /**------------------------------------------------------------------------
     * Construct a buffer of specified dimensions.
     * The contents are initialised to zero.
     *
     *------------------------------------------------------------------------*/
    Buffer(int num_channels, int num_frames);

    /**------------------------------------------------------------------------
     * Construct a buffer of specified dimensions.
     * The contents of `data` are copied into the buffer, where `data` must
     * be an array of `num_channels` pointers to individual audio channels.
     *
     *------------------------------------------------------------------------*/
    Buffer(int num_channels, int num_frames, sample **data);

    /**------------------------------------------------------------------------
     * Construct a buffer of specified dimensions.
     * The contents of `data` are copied into the buffer, where `data` must
     * be a vector of `num_channels` audio channels.
     *
     *------------------------------------------------------------------------*/
    Buffer(int num_channels, int num_frames, std::vector<std::vector<sample>> data);

    /**------------------------------------------------------------------------
     * Construct a buffer with the same dimensions as `data`.
     * The contents of `data` are copied into the buffer.
     *
     *------------------------------------------------------------------------*/
    Buffer(std::vector<std::vector<sample>> data);

    /**------------------------------------------------------------------------
     * Construct a single-channel buffer with the contents of `data`.
     *
     *------------------------------------------------------------------------*/
    Buffer(std::vector<sample> data);

    /**------------------------------------------------------------------------
     * Load the contents of the audio file `filename` into a new buffer.
     * The file must be of a format that libsndfile can read.
     *
     *------------------------------------------------------------------------*/
    Buffer(std::string filename);

    /**------------------------------------------------------------------------
      * Destroy the buffer.
      *
      *------------------------------------------------------------------------*/
    virtual ~Buffer();

    /**------------------------------------------------------------------------
      * Resize the buffer allocation.
      *
      *------------------------------------------------------------------------*/
    void resize(int num_channels, int num_frames);

    /**------------------------------------------------------------------------
     * Load the contents of `filename` into the buffer.
     * If the buffer is smaller than the file's contents, only the first
     * part of the file is read.
     *
     * @param filename The filename to read. Must be of a type supported by
     *                 libsndfile.
     *
     *------------------------------------------------------------------------*/
    void load(std::string filename);

    /**------------------------------------------------------------------------
     * Write the contents of the buffer to the file `filename`.
     * Only supports .wav format at present.
     *
     * @param filename The filename to write. Must end in .wav.
     *
     *------------------------------------------------------------------------*/
    void save(std::string filename);

    /**------------------------------------------------------------------------
     * Splits the buffer into chunks of `num_frames_per_part` frames,
     * and returns the vector of chunks. Useful for creating Buffer2D
     * objects from longer 1D wavetable buffers.
     *
     * TODO split(N) would normally denote how many parts to split into.
     *      split(chunk_size=) / split(split_count=) ?
     *
     *------------------------------------------------------------------------*/
    std::vector<BufferRef> split(int num_frames_per_part);

    /**------------------------------------------------------------------------
     * @param channel The channel number
     * @param index A sample offset, between [0, num_frames].
     * @returns A sample value, between [-1, 1].
     *
     *------------------------------------------------------------------------*/
    sample get(int channel, double offset);

    /**------------------------------------------------------------------------
     * @param frame The absolute frame value to retrieve.
     * @return The raw value stored within that frame.
     *
     *------------------------------------------------------------------------*/
    sample get_frame(int channel, double frame);

    /**------------------------------------------------------------------------
     * @param frame_index The frame index to set
     * @param value The sample value
     * @returns true if the set succeeded, false if frame_index is out of range
     *
     *------------------------------------------------------------------------*/
    bool set(int channel_index,
             int frame_index,
             sample value);

    /**------------------------------------------------------------------------
     * Map a frame index to an offset in the buffer's native range.
     *
     * @param frame The frame
     * @returns The offset
     *
     *------------------------------------------------------------------------*/
    virtual double frame_to_offset(double frame);

    /**------------------------------------------------------------------------
     * Map an offset to a frame value.
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset);

    /**------------------------------------------------------------------------
     * Fill the buffer with a constant value.
     *
     * @param value The value to fill the buffer with
     *
     *------------------------------------------------------------------------*/
    void fill(sample value);

    /**------------------------------------------------------------------------
     * Fill the buffer based on a transfer function, which maps the offset
     * of each sample to a new output value.
     *
     * @param f A function which takes a single parameter, which is passed
     *          the offset of each sample.
     *
     * For example, this can be used to create a quadratic waveshaper:
     *
     * buffer->fill([](float input) { return input * input; });
     *
     *------------------------------------------------------------------------*/
    void fill(const std::function<float(float)> f);

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
     * Get the number of channels in the buffer.
     *
     * @returns The number of channels.
     *
     *------------------------------------------------------------------------*/
    unsigned int get_num_channels();

    /**------------------------------------------------------------------------
     * Get the number of frames in the buffer.
     *
     * @returns The number of frames.
     *
     *------------------------------------------------------------------------*/
    unsigned long get_num_frames();

    /**------------------------------------------------------------------------
     * Get the duration of the buffer.
     *
     * @returns The duration, in seconds.
     *
     *------------------------------------------------------------------------*/
    float get_duration();

    /**------------------------------------------------------------------------
     * Get the filename that the buffer was loaded from / saved to, if set.
     *
     * @returns The filename, or an empty string.
     *
     *------------------------------------------------------------------------*/
    std::string get_filename();

    /**------------------------------------------------------------------------
     * Get a pointer to the buffer's data.
     * Each element points to a `sample *` containing a channel of audio.
     *
     * @returns The pointer.
     *
     *------------------------------------------------------------------------*/
    sample **get_data();

    /**------------------------------------------------------------------------
     * Set the interpolation mode used when samples are read from the buffer.
     *
     * @param mode The new interpolation mode.
     *
     *------------------------------------------------------------------------*/
    void set_interpolation_mode(signalflow_interpolation_mode_t mode);

    /**------------------------------------------------------------------------
     * Gets the interpolation mode used when samples are read from the buffer.
     *
     * @returns The current interpolation mode.
     *
     *------------------------------------------------------------------------*/
    signalflow_interpolation_mode_t get_interpolation_mode();

    /**------------------------------------------------------------------------
     * Subscript operator is a direct accessor into the buffer's data.
     * Each array element points to a single channel of floating-point samples.
     *
     * @param index Array index. Must be smaller than the buffer's
     *              channel count.
     * @returns A reference to a channel of samples
     *
     *------------------------------------------------------------------------*/
    sample *&operator[](int index);

    sample **data = nullptr;

protected:
    std::string filename;
    float sample_rate;
    unsigned int num_channels;
    unsigned long num_frames;
    float duration;

    signalflow_interpolation_mode_t interpolate;
};

/**-------------------------------------------------------------------------
 * An EnvelopeBuffer is a mono buffer with a fixed number of samples,
 * which can be sampled at a position [0,1] to give an amplitude value.
 * Mostly intended to be subclassed.
 *-----------------------------------------------------------------------*/
class EnvelopeBuffer : public Buffer
{
public:
    EnvelopeBuffer(int length = SIGNALFLOW_DEFAULT_ENVELOPE_BUFFER_LENGTH);

    /**------------------------------------------------------------------------
      * Initialise a buffer with the envelope named `name`.
      *
      * The list of supported buffer identifiers:
      *
      *   hanning: Hanning envelope
      *   triangle: Symmetrical linear ramp from 0..1..0
      *   linear-decay: Linear decay from 1..0
      *
      * @param name One of the recognised names above
      * @param num_frames Length of buffer
      *
      *------------------------------------------------------------------------*/
    EnvelopeBuffer(std::string name, int num_frames = SIGNALFLOW_DEFAULT_ENVELOPE_BUFFER_LENGTH);

    /**------------------------------------------------------------------------
     * @param position An envelope position between [0, 1].
     * @return An envelope amplitude value, between [0, 1].
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset) override;
    virtual double frame_to_offset(double frame) override;
};

/*-------------------------------------------------------------------------
 * A WaveShaperBuffer is a mono buffer with a fixed number of samples
 * that can be sampled at a position [-1,1] to give an amplitude mapping
 * value, equal to the result of the shaper's transfer function for
 * that value.
 *-----------------------------------------------------------------------*/
class WaveShaperBuffer : public Buffer
{
public:
    WaveShaperBuffer(int length = SIGNALFLOW_DEFAULT_ENVELOPE_BUFFER_LENGTH);

    /**------------------------------------------------------------------------
     * Perform a waveshaper x -> f(x) transform.
     *
     * @param input A given input sample, between [-1, 1]
     * @return A transformed sample value, between [-1, 1].
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset) override;
    virtual double frame_to_offset(double frame) override;
};

template <class T>
class BufferRefTemplate : public std::shared_ptr<T>
{
public:
    using std::shared_ptr<T>::shared_ptr;

    BufferRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    BufferRefTemplate(T *ptr)
        : std::shared_ptr<T>(ptr) {}
    BufferRefTemplate operator*(double constant);
};

}
