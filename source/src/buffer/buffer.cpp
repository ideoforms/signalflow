#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/exceptions.h"
#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

#ifdef HAVE_SNDFILE
#include <sndfile.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>

#define SIGNALFLOW_DEFAULT_BUFFER_BLOCK_SIZE 1024

namespace signalflow
{

extern AudioGraph *shared_graph;

Buffer::Buffer()
    : Buffer(0, 0)
{
}

Buffer::Buffer(int num_channels, int num_frames)
{
    if (shared_graph == NULL)
    {
        throw signalflow::graph_not_created_exception("AudioGraph must be created before Buffer can be allocated");
    }

    this->num_channels = num_channels;
    this->num_frames = num_frames;
    this->sample_rate = shared_graph->get_sample_rate();
    this->duration = this->num_frames / this->sample_rate;
    this->interpolate = SIGNALFLOW_INTERPOLATION_LINEAR;

    // contiguous allocation
    if (this->num_channels)
    {
        this->data = new sample *[this->num_channels]();
        sample *data_channels = new sample[this->num_channels * this->num_frames]();
        for (int channel = 0; channel < this->num_channels; channel++)
        {
            this->data[channel] = data_channels + (this->num_frames * channel);
        }
    }
    else
    {
        this->data = 0;
    }
}

Buffer::Buffer(int num_channels, int num_frames, sample **data)
    : Buffer(num_channels, num_frames)
{
    for (int channel = 0; channel < this->num_channels; channel++)
    {
        memcpy(this->data[channel], data[channel], num_frames * sizeof(sample));
    }
}

Buffer::Buffer(int num_channels, int num_frames, std::vector<std::vector<sample>> data)
    : Buffer(num_channels, num_frames)
{
    for (int channel = 0; channel < this->num_channels; channel++)
    {
        std::copy(data[channel].begin(), data[channel].end(), this->data[channel]);
    }
}

Buffer::Buffer(std::vector<std::vector<sample>> data)
    : Buffer(data.size(), data[0].size(), data)
{
}

Buffer::Buffer(std::vector<sample> data)
    : Buffer(1, data.size(), std::vector<std::vector<sample>>({ data }))
{
}

Buffer::Buffer(std::string filename)
{
    this->interpolate = SIGNALFLOW_INTERPOLATION_LINEAR;
    this->load(filename);
}

Buffer::~Buffer()
{
    if (this->data)
    {
        delete this->data[0];
        delete this->data;
    }
}

void Buffer::load(std::string filename)
{
#ifdef HAVE_SNDFILE

    std::string path = filename;
    if (access(path.c_str(), F_OK) != 0)
    {
        path = SIGNALFLOW_USER_DIR + "/audio/" + filename;
        if (access(path.c_str(), F_OK) != 0)
        {
            throw std::runtime_error(std::string("Couldn't find file at path: ") + filename);
        }
    }

    SF_INFO info;
    SNDFILE *sndfile = sf_open(path.c_str(), SFM_READ, &info);

    if (!sndfile)
    {
        throw std::runtime_error(std::string("Couldn't read audio from path: ") + filename);
    }

    if (this->data)
    {
        /*------------------------------------------------------------------------
         * If the buffer has already been allocated, we want to read as many
         * frames as possible into the existing allocation. Check that the
         * existing config is compatible with the audio file.
         *-----------------------------------------------------------------------*/
        if (this->num_channels != info.channels)
        {
            throw std::runtime_error(std::string("Can't read audio: audio file channel count does not match buffer"));
        }
        if (this->sample_rate != info.samplerate)
        {
            throw std::runtime_error(std::string("Can't read audio: audio file sample rate does not match buffer"));
        }
    }
    else
    {
        /*------------------------------------------------------------------------
         * Buffer has not yet been allocated. Allocate memory and populate
         * property fields.
         * TODO: Sample rate isn't properly handled yet.
         *-----------------------------------------------------------------------*/

        // Contiguous allocation is required for Python bindings
        this->data = new sample *[info.channels]();
        sample *data_channels = new sample[info.channels * (info.frames + SIGNALFLOW_DEFAULT_BUFFER_BLOCK_SIZE)]();

        for (int channel = 0; channel < info.channels; channel++)
        {
            this->data[channel] = data_channels + (info.frames + SIGNALFLOW_DEFAULT_BUFFER_BLOCK_SIZE) * channel;
        }

        this->num_channels = info.channels;
        this->num_frames = info.frames;
        this->sample_rate = info.samplerate;
        this->duration = this->num_frames / this->sample_rate;
    }

    int frames_per_read = SIGNALFLOW_DEFAULT_BUFFER_BLOCK_SIZE;
    int samples_per_read = frames_per_read * info.channels;
    sample *buffer = new sample[samples_per_read];
    int total_frames_read = 0;

    while (true)
    {
        int count = sf_readf_float(sndfile, buffer, frames_per_read);
        for (int frame = 0; frame < count; frame++)
        {
            // TODO: Vector-accelerated de-interleave
            for (int channel = 0; channel < info.channels; channel++)
            {
                this->data[channel][total_frames_read] = buffer[frame * info.channels + channel];
            }
            total_frames_read++;

            /*------------------------------------------------------------------------
             * Check whether we've hit the limit of this Buffer, which can happen
             * in the case of pre-allocated buffers loading a determinate # samples
             * from memory.
             *-----------------------------------------------------------------------*/
            if (total_frames_read >= this->num_frames)
            {
                break;
            }
        }
        if (count < frames_per_read)
        {
            break;
        }
    }

    delete[] buffer;

    sf_close(sndfile);

    // TODO Logging
    // std::cout << "Read " << info.channels << " channels, " << info.frames << " frames" << std::endl;

#else

    throw std::runtime_error("Can't load audio data as libsndfile was not detected at compile time");

#endif
}

void Buffer::save(std::string filename)
{
#ifdef HAVE_SNDFILE

    SF_INFO info;
    memset(&info, 0, sizeof(SF_INFO));
    info.frames = this->num_frames;
    info.channels = this->num_channels;
    info.samplerate = (int) this->sample_rate;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    SNDFILE *sndfile = sf_open(filename.c_str(), SFM_WRITE, &info);

    if (!sndfile)
    {
        throw std::runtime_error(std::string("Failed to write soundfile (") + std::string(sf_strerror(NULL)) + ")");
    }

    int frames_per_write = SIGNALFLOW_DEFAULT_BUFFER_BLOCK_SIZE;
    int samples_per_write = frames_per_write * info.channels;
    sample *buffer = new sample[samples_per_write];
    int frame_index = 0;

    while (true)
    {
        int frames_this_write = frames_per_write;
        if (this->num_frames - frame_index < frames_this_write)
            frames_this_write = this->num_frames - frame_index;

        for (int frame = 0; frame < frames_this_write; frame++)
        {
            // TODO: Vector-accelerated interleave
            for (int channel = 0; channel < info.channels; channel++)
            {
                buffer[frame * info.channels + channel] = this->data[channel][frame_index];
            }
            frame_index++;
        }
        sf_writef_float(sndfile, buffer, frames_this_write);
        if (frame_index >= this->num_frames)
            break;
    }

    delete[] buffer;

    sf_close(sndfile);

#else

    throw std::runtime_error("Can't save audio data as libsndfile was not detected at compile time");

#endif
}

std::vector<BufferRef> Buffer::split(int num_frames_per_part)
{
    if (this->num_channels != 1)
    {
        throw std::runtime_error("Buffer::split currently only supports mono buffers");
    }

    int buffer_count = this->num_frames / num_frames_per_part;
    std::vector<BufferRef> bufs(buffer_count);
    for (int i = 0; i < buffer_count; i++)
    {
        sample *ptr = this->data[0] + (i * num_frames_per_part);
        BufferRef buf = new Buffer(1, num_frames_per_part, &ptr);

        // Is there a better way to initialise all properties of the buffer?
        buf->interpolate = this->interpolate;

        bufs[i] = buf;
    }
    return bufs;
}

double Buffer::frame_to_offset(double frame)
{
    return (double) frame;
}

double Buffer::offset_to_frame(double offset)
{
    return (double) offset;
}

bool Buffer::set(int channel_index,
                 int frame_index,
                 sample value)
{
    if (channel_index >= 0 && channel_index < this->num_channels && frame_index >= 0 && frame_index < this->num_frames)
    {
        this->data[channel_index][frame_index] = value;
        return true;
    }
    else
    {
        return false;
    }
}

sample Buffer::get_frame(double frame)
{
    if (!this->data)
    {
        throw std::runtime_error("Buffer has zero length, frame is out of bounds");
    }

    if (frame > this->num_frames - 1)
    {
        frame = this->num_frames - 1;
    }
    else if (frame < 0)
    {
        frame = 0;
    }

    if (this->interpolate == SIGNALFLOW_INTERPOLATION_LINEAR)
    {
        double frame_frac = (frame - (int) frame);
        sample rv = ((1.0 - frame_frac) * this->data[0][(int) frame]) + (frame_frac * this->data[0][(int) ceil(frame)]);
        return rv;
    }
    else
    {
        return this->data[0][(int) frame];
    }
}

sample Buffer::get(double offset)
{
    double frame = this->offset_to_frame(offset);
    return this->get_frame(frame);
}

void Buffer::fill(sample value)
{
    for (int channel = 0; channel < this->num_channels; channel++)
    {
        for (int frame = 0; frame < this->num_frames; frame++)
        {
            this->data[channel][frame] = value;
        }
    }
}

void Buffer::fill(const std::function<float(float)> f)
{
    for (int channel = 0; channel < this->num_channels; channel++)
    {
        for (int frame = 0; frame < this->num_frames; frame++)
        {
            double offset = this->frame_to_offset(frame);
            this->data[channel][frame] = f(offset);
        }
    }
}

float Buffer::get_sample_rate()
{
    return this->sample_rate;
}

int Buffer::get_num_channels()
{
    return this->num_channels;
}

int Buffer::get_num_frames()
{
    return this->num_frames;
}

float Buffer::get_duration()
{
    return this->duration;
}

void Buffer::set_interpolation_mode(signalflow_interpolation_mode_t mode)
{
    this->interpolate = mode;
}

signalflow_interpolation_mode_t Buffer::get_interpolation_mode()
{
    return this->interpolate;
}

sample **Buffer::get_data()
{
    return this->data;
}

sample *&Buffer::operator[](int index)
{
    return this->data[index];
}

template <class T>
BufferRefTemplate<T> BufferRefTemplate<T>::operator*(double constant)
{
    Buffer *buffer = (Buffer *) this->get();

    std::vector<std::vector<sample>> output(buffer->get_num_channels());
    for (int channel = 0; channel < buffer->get_num_channels(); channel++)
    {
        output[channel].resize(buffer->get_num_frames());
        for (int frame = 0; frame < buffer->get_num_frames(); frame++)
        {
            output[channel][frame] = buffer->data[channel][frame] * constant;
        }
    }
    return new Buffer(buffer->get_num_channels(), buffer->get_num_frames(), output);
}

template class BufferRefTemplate<Buffer>;

EnvelopeBuffer::EnvelopeBuffer(int length)
    : Buffer(1, length)
{
    /*-------------------------------------------------------------------------
     * Initialise to a flat envelope at maximum amplitude.
     *-----------------------------------------------------------------------*/
    this->fill(1.0);
}

double EnvelopeBuffer::offset_to_frame(double offset)
{
    return signalflow_scale_lin_lin(offset, 0, 1, 0, this->num_frames - 1);
}

double EnvelopeBuffer::frame_to_offset(double frame)
{
    return signalflow_scale_lin_lin(frame, 0, this->num_frames - 1, 0, 1);
}

EnvelopeBufferTriangle::EnvelopeBufferTriangle(int length)
    : EnvelopeBuffer(length)
{
    for (int x = 0; x < length / 2; x++)
        this->data[0][x] = (float) x / (length / 2);
    for (int x = 0; x < length / 2; x++)
        this->data[0][(length / 2) + x] = 1.0 - (float) x / (length / 2);
}

EnvelopeBufferLinearDecay::EnvelopeBufferLinearDecay(int length)
    : EnvelopeBuffer(length)
{
    for (int x = 0; x < length; x++)
        this->data[0][x] = 1.0 - (float) x / length;
}

EnvelopeBufferHanning::EnvelopeBufferHanning(int length)
    : EnvelopeBuffer(length)
{
    for (int x = 0; x < length; x++)
    {
        this->data[0][x] = 0.5 * (1.0 - cos(2 * M_PI * x / (length - 1)));
    }
}

WaveShaperBuffer::WaveShaperBuffer(int length)
    : Buffer(1, length)
{
    /*-------------------------------------------------------------------------
     * Initialise to a 1-to-1 linear mapping.
     *-----------------------------------------------------------------------*/
    this->fill([](float input) { return input; });
}

double WaveShaperBuffer::offset_to_frame(double offset)
{
    return signalflow_scale_lin_lin(offset, -1, 1, 0, this->num_frames - 1);
}

double WaveShaperBuffer::frame_to_offset(double frame)
{
    return signalflow_scale_lin_lin(frame, 0, this->num_frames - 1, -1, 1);
}

}
