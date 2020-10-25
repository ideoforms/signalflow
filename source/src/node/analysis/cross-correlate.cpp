#include "signalflow/core/graph.h"
#include "signalflow/node/analysis/cross-correlate.h"

#include <stdlib.h>

namespace signalflow
{

CrossCorrelate::CrossCorrelate(NodeRef input, BufferRef buffer, int hop_size)
    : UnaryOpNode(input), buffer(buffer), hop_size(hop_size)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "cross-correlate";

    this->create_buffer("buffer", this->buffer);
    this->create_input("input", this->input);
    this->ring_buffer = new SampleRingBuffer(buffer->get_num_frames());
}

void CrossCorrelate::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    int buffer_num_frames = this->buffer->get_num_frames();
    this->ring_buffer->extend(this->input->out[0], num_frames);

    float correlation;
    for (int hop = 0; hop < num_frames; hop += this->hop_size)
    {
        correlation = 0;
        for (int frame = 0; frame < buffer_num_frames; frame++)
        {
            correlation += this->buffer->data[0][frame] * this->ring_buffer->get(-2 * buffer_num_frames + hop + frame);
        }
        for (int frame = 0; frame < this->hop_size; frame++)
        {
            for (int channel = 0; channel < this->num_output_channels; channel++)
            {
                out[channel][hop * this->hop_size + frame] = correlation;
            }
        }
    }
}

}
