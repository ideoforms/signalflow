#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/feedback-buffer-writer.h"

namespace signalflow
{

FeedbackBufferWriter::FeedbackBufferWriter(BufferRef buffer, NodeRef input, NodeRef delay_time)
    : buffer(buffer), input(input), delay_time(delay_time)
{
    if (!buffer)
    {
        throw std::runtime_error("No buffer specified");
    }

    this->name = "feedback-buffer-writer";

    this->create_buffer("buffer", this->buffer);
    this->create_input("input", this->input);
    this->create_input("delay_time", this->delay_time);

    this->phase = 0.0;

    this->set_channels(buffer->get_num_channels(), 0);
}

void FeedbackBufferWriter::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_input_channels; channel++)
        {
            int delay_samples = (int) (this->delay_time->out[channel][frame] * this->graph->get_sample_rate());

            if (delay_samples < this->graph->get_output_buffer_size())
            {
                signalflow_audio_thread_error("FeedbackBufferWriter delay_time cannot be shorter than the AudioGraph output buffer size");
            }

            int offset = this->phase + delay_samples;
            offset = offset % buffer->get_num_frames();
            this->buffer->data[channel][offset] = this->input->out[channel][frame];
        }

        this->phase += 1;
        while (this->phase >= this->buffer->get_num_frames())
            this->phase -= this->buffer->get_num_frames();
    }
}

}
