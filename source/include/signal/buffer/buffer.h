#pragma once

/**-------------------------------------------------------------------------
 * @file buffer.h
 * @brief Buffer and its subclasses store an array of sample values.
 *-----------------------------------------------------------------------*/
#include "signal/core/constants.h"
#include "signal/core/util.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define SIGNAL_DEFAULT_ENVELOPE_BUFFER_LENGTH 2048

/**------------------------------------------------------------------------
 * Typedef for a sample -> sample transfer function.
 * Convenient for lambda-based features.
 *------------------------------------------------------------------------*/
typedef sample (*transfer_fn)(sample);

namespace libsignal
{

template <class T>
class BufferRefTemplate;
class Buffer;
typedef BufferRefTemplate<Buffer> BufferRef;

class Buffer
{
public:
    Buffer();
    Buffer(int num_channels, int num_frames);
    Buffer(int num_channels, int num_frames, sample **data);
    Buffer(int num_channels, int num_frames, std::vector<std::vector<sample>> data);
    Buffer(std::vector<std::vector<sample>> data);
    Buffer(std::vector<sample> data);

    Buffer(std::string filename);

    virtual ~Buffer();

    void load(std::string filename);
    void save(std::string filename);

    // TODO split(N) would normally denote how many parts to split into.
    //      split(chunk_size=) / split(split_count=) ?
    std::vector<BufferRef> split(int num_frames_per_part);

    /**------------------------------------------------------------------------
    * @param index A sample offset, between [0, num_frames].
    * @return A sample value, between [-1, 1].
    *------------------------------------------------------------------------*/
    sample get(double offset);

    /**------------------------------------------------------------------------
     * @param frame The absolute frame value to retrieve.
     * @return The raw value stored within that frame.
     * TODO Add support for buffers with >1 channel.
     *------------------------------------------------------------------------*/
    sample get_frame(double frame);

    /**------------------------------------------------------------------------
     * @param frame_index The frame index to set
     * @param value The sample value
     * @return true if the set succeeded, false if frame_index is out of range
     *------------------------------------------------------------------------*/
    bool set(int channel_index,
             int frame_index,
             sample value);

    /**------------------------------------------------------------------------
     * Map a frame index to an offset in the buffer's native range.
     *------------------------------------------------------------------------*/
    virtual double frame_to_offset(double frame);

    /**------------------------------------------------------------------------
     * Map an offset to a frame value.
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset);

    /**------------------------------------------------------------------------
     * Fill the buffer with a constant value.
     *------------------------------------------------------------------------*/
    void fill(sample value);

    /**------------------------------------------------------------------------
     * Fill the buffer based on a transfer function
     *------------------------------------------------------------------------*/
    void fill(transfer_fn f);

    float sample_rate;
    int num_channels;
    int num_frames;
    sample **data = NULL;

    float duration;
    signal_interpolation_mode_t interpolate;
};

/**-------------------------------------------------------------------------
 * An EnvelopeBuffer is a mono buffer with a fixed number of samples,
 * which can be sampled at a position [0,1] to give an amplitude value.
 * Mostly intended to be subclassed.
 *-----------------------------------------------------------------------*/
class EnvelopeBuffer : public Buffer
{
public:
    EnvelopeBuffer(int length = SIGNAL_DEFAULT_ENVELOPE_BUFFER_LENGTH);

    /**------------------------------------------------------------------------
     * @param position An envelope position between [0, 1].
     * @return An envelope amplitude value, between [0, 1].
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset) override;
    virtual double frame_to_offset(double frame) override;
    virtual void fill_exponential(float mu);
    virtual void fill_beta(float a, float b);
};

class EnvelopeBufferTriangle : public EnvelopeBuffer
{
public:
    EnvelopeBufferTriangle(int length = SIGNAL_DEFAULT_ENVELOPE_BUFFER_LENGTH);
};

class EnvelopeBufferLinearDecay : public EnvelopeBuffer
{
public:
    EnvelopeBufferLinearDecay(int length = SIGNAL_DEFAULT_ENVELOPE_BUFFER_LENGTH);
};

class EnvelopeBufferHanning : public EnvelopeBuffer
{
public:
    EnvelopeBufferHanning(int length = SIGNAL_DEFAULT_ENVELOPE_BUFFER_LENGTH);
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
    WaveShaperBuffer(int length = SIGNAL_DEFAULT_ENVELOPE_BUFFER_LENGTH);

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
