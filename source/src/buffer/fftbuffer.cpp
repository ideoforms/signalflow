#include "signalflow/buffer/fftbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/exceptions.h"
#include "signalflow/core/graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <vector>

namespace signalflow
{

extern AudioGraph *shared_graph;

FFTBuffer::FFTBuffer(std::string filename, int fft_size, int hop_size)
    : fft_size(fft_size), hop_size(hop_size)
{
    this->num_bins = (fft_size / 2) + 1;

    FILE *fd = fopen(filename.c_str(), "r");
    if (fd == NULL)
    {
        throw std::runtime_error(std::string("Couldn't find file at path: ") + filename);
    }

    fseek(fd, 0, SEEK_END);
    size_t file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    double num_frames_frac = (double) file_size / (this->num_bins * 2 * sizeof(float));
    printf("FFTBuffer: File size %zu bytes, %.2f frames\n", file_size, num_frames_frac);
    if (num_frames_frac != (int) num_frames_frac)
    {
        throw std::runtime_error("Error: Not an integer number of frames (found " + std::to_string(num_frames) + " frames)");
    }
    this->num_frames = (unsigned int) num_frames_frac;

    /*--------------------------------------------------------------------------------
     * If the AudioGraph has been instantiated, populate the buffer's sample
     * rate and duration. Otherwise, zero them.
     *-------------------------------------------------------------------------------*/
    if (shared_graph)
    {
        this->sample_rate = shared_graph->get_sample_rate();
        this->duration = (this->num_frames * this->hop_size) / this->sample_rate;
    }
    else
    {
        this->sample_rate = 0;
        this->duration = 0;
    }

    this->resize(num_frames);

    for (int frame = 0; frame < num_frames; frame++)
    {
        fread(this->data[frame], sizeof(float), this->num_bins * 2, fd);
    }
}

FFTBuffer::~FFTBuffer()
{
    if (this->data)
    {
        delete this->data[0];
        delete this->data;

        if (shared_graph)
        {
            size_t num_bytes = this->get_total_num_values() * sizeof(sample);
            shared_graph->register_memory_dealloc(num_bytes);
        }
    }
}

void FFTBuffer::resize(int num_frames)
{
    if (this->data)
    {
        delete this->data[0];
        delete this->data;

        if (shared_graph)
        {
            size_t num_bytes = this->get_total_num_values() * sizeof(sample);
            shared_graph->register_memory_dealloc(num_bytes);
        }
    }

    this->num_frames = num_frames;

    /*--------------------------------------------------------------------------------
     * For use in numpy, memory allocation needs to be contiguous with a fixed
     * stride between vectors. Allocate as one block and set element indices
     * accordingly.
     *-------------------------------------------------------------------------------*/
    if (num_frames)
    {
        this->data = new sample *[this->num_frames]();

        sample *data_frames = new sample[this->get_total_num_values()]();
        for (unsigned int frame = 0; frame < this->num_frames; frame++)
        {
            this->data[frame] = data_frames + frame * (this->num_bins * 2);
        }

        if (shared_graph)
        {
            size_t num_bytes = this->get_total_num_values() * sizeof(sample);
            shared_graph->register_memory_alloc(num_bytes);
        }
    }
    else
    {
        this->data = nullptr;
    }
}

sample *FFTBuffer::get_frame(int frame) { return this->data[frame]; }

float FFTBuffer::get_sample_rate() { return this->sample_rate; }

void FFTBuffer::set_sample_rate(float sample_rate) { this->sample_rate = sample_rate; }

unsigned long FFTBuffer::get_num_frames() { return this->num_frames; }

std::string FFTBuffer::get_filename() { return this->filename; }

unsigned int FFTBuffer::get_fft_size() { return this->fft_size; }

unsigned int FFTBuffer::get_hop_size() { return this->hop_size; }

unsigned int FFTBuffer::get_num_bins() { return this->num_bins; }

float FFTBuffer::get_duration() { return this->duration; }

unsigned long FFTBuffer::get_total_num_values() { return this->num_frames * this->num_bins * 2; }

template class FFTBufferRefTemplate<FFTBuffer>;

}
