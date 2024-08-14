#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/history-buffer-writer.h"

namespace signalflow
{

HistoryBufferWriter::HistoryBufferWriter(BufferRef buffer, NodeRef input, int downsample)
    : UnaryOpNode(input), buffer(buffer), downsample(downsample)
{
    if (!buffer)
    {
        throw std::runtime_error("No buffer specified");
    }
    if (downsample < 1)
    {
        throw std::runtime_error("Invalid value for downsample (must be 1 or above)");
    }

    this->name = "history-buffer-writer";

    this->create_buffer("buffer", this->buffer);
    this->set_channels(buffer->get_num_channels(),
                       buffer->get_num_channels());
}

void HistoryBufferWriter::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    /*--------------------------------------------------------------------------------
     * Basic nearest-neighbour downsampling. Might want something fancier
     * in future.
     *--------------------------------------------------------------------------------*/
    int num_frames_downsampled = num_frames / this->downsample;
    size_t num_overlap_frames = this->buffer->get_num_frames() - num_frames_downsampled;

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        memmove(this->buffer->data[channel],
                this->buffer->data[channel] + num_frames_downsampled,
                num_overlap_frames * sizeof(sample));
        for (int frame = 0; frame < num_frames_downsampled; frame++)
        {
            this->buffer->data[channel][num_overlap_frames + frame] = this->input->out[channel][frame * downsample];
        }

        memcpy(this->out.data[channel],
               this->input->out[channel],
               num_frames * sizeof(sample));
    }
}

}
