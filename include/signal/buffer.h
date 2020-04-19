#pragma once 

/**-------------------------------------------------------------------------
 * @file buffer.h
 * @brief Buffer and its subclasses store an array of sample values.
 *-----------------------------------------------------------------------*/
#include "constants.h"
#include "util.h"

#include <math.h>
#include <memory>

#define SIGNAL_ENVELOPE_BUFFER_LENGTH 1024
#define SIGNAL_ENVELOPE_BUFFER_HALF_LENGTH (SIGNAL_ENVELOPE_BUFFER_LENGTH / 2)

typedef enum
{
    SIGNAL_INTERPOLATE_NONE,
    SIGNAL_INTERPOLATE_LINEAR
} signal_interpolate_t;

/**------------------------------------------------------------------------
 * Typedef for a sample -> sample transfer function.
 * Convenient for lambda-based features.
 *------------------------------------------------------------------------*/
typedef sample(*transfer_fn)(sample);

namespace libsignal
{
    class Buffer
    {
    public:
        Buffer();
        Buffer(int num_channels, int num_frames, sample **data);
        Buffer(int num_channels, int num_frames, std::vector<std::vector<sample>>data);
        Buffer(int num_channels, int num_frames);
        Buffer(const char *filename);

        virtual ~Buffer();

        void open(const char *filename);
        void save(const char *filename);

        float sample_rate;
        int num_channels;
        int num_frames;
        sample **data = NULL;

        float duration;
        signal_interpolate_t interpolate;

        /**------------------------------------------------------------------------
         * Map a frame index to an offset in the buffer's native range.
         *------------------------------------------------------------------------*/
        virtual double frame_to_offset(double frame);

        /**------------------------------------------------------------------------
         * Map an offset to a frame value.
         *------------------------------------------------------------------------*/
        virtual double offset_to_frame(double offset);

        /**------------------------------------------------------------------------
         * @param frame The absolute frame value to retrieve.
         * @return The raw value stored within that frame.
         *------------------------------------------------------------------------*/
        sample get_frame(double frame);

        /**------------------------------------------------------------------------
         * @param index A sample offset, between [0, num_frames].
         * @return A sample value, between [-1, 1].
         *------------------------------------------------------------------------*/
        virtual sample get(double offset);

        /**------------------------------------------------------------------------
         * Fill the buffer with a constant value.
         *------------------------------------------------------------------------*/
        void fill(sample value);

        /**------------------------------------------------------------------------
         * Fill the buffer based on a transfer function
         *------------------------------------------------------------------------*/
        void fill(transfer_fn f);
    };

    /**-------------------------------------------------------------------------
     * An EnvelopeBuffer is a mono buffer with a fixed number of samples,
     * which can be sampled at a position [0,1] to give an amplitude value.
     * Mostly intended to be subclassed.
     *-----------------------------------------------------------------------*/
    class EnvelopeBuffer : public Buffer
    {
        public:
            EnvelopeBuffer(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH);

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
            EnvelopeBufferTriangle(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH);
    };

    class EnvelopeBufferLinearDecay : public EnvelopeBuffer
    {
        public:
            EnvelopeBufferLinearDecay(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH);
    };

    class EnvelopeBufferHanning : public EnvelopeBuffer
    {
        public:
            EnvelopeBufferHanning(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH);
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
            WaveShaperBuffer(int length = SIGNAL_ENVELOPE_BUFFER_LENGTH);

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

        BufferRefTemplate() : std::shared_ptr<T>(nullptr) { }
        BufferRefTemplate(T *ptr) : std::shared_ptr<T>(ptr) { }
    };

    typedef BufferRefTemplate<Buffer> BufferRef;

    class InterpolatingBuffer2D : public Buffer
    {
        public:
            InterpolatingBuffer2D(BufferRef bufferA, BufferRef bufferB);

            /**------------------------------------------------------------------------
             * Perform a waveshaper x -> f(x) transform.
             *
             * @param offset Frame #
             * @param crossfade Fade between bufferA and bufferB, between [0, 1]
             * @return A transformed sample value, between [-1, 1].
             *------------------------------------------------------------------------*/
            sample get2D(double offset, double crossfade);

        private:
            BufferRef bufferA;
            BufferRef bufferB;
    };

    typedef BufferRefTemplate<InterpolatingBuffer2D> BufferRef2D;
}
