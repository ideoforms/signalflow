#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/feedback-buffer-reader.h"

#include <stdlib.h>

namespace signalflow
{

FeedbackBufferReader::FeedbackBufferReader(BufferRef buffer)
    : Node()
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "feedback-buffer-reader";

    this->buffer = buffer;
    this->create_buffer("buffer", this->buffer);
    this->set_channels(1, 0);

    if (buffer)
    {
        /*--------------------------------------------------------------------------------
         * buffer can be null when a FeedbackBufferReader is being instantiated as part of
         * a Patch. Its buffer is populated later.
         *--------------------------------------------------------------------------------*/
        this->set_buffer("buffer", buffer);
    }
}

void FeedbackBufferReader::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        Node::set_buffer(name, buffer);
        this->num_output_channels = buffer->get_num_channels();
        this->rate_scale_factor = buffer->get_sample_rate() / graph->get_sample_rate();
    }
}

void FeedbackBufferReader::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    int buf_num_frames = this->buffer->get_num_frames();

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if ((int) this->phase >= buf_num_frames)
            {
                this->phase = 0;
            }

            out[channel][frame] = this->buffer->get_frame(channel, phase);
            this->phase += 1;
        }
    }
}

}
