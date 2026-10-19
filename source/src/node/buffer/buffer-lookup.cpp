#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/buffer-lookup.h"

namespace signalflow
{

BufferLookup::BufferLookup(BufferRef buffer, NodeRef index)
    : Node(), index(index)
{
    this->name = "buffer-lookup";

    this->create_buffer("buffer", this->buffer);
    this->create_input("index", this->index);
    this->set_channels(1, 0);

    if (buffer)
    {
        /*--------------------------------------------------------------------------------
         * buffer can be null when a BufferLookup is being instantiated as part of
         * a Patch. Its buffer is populated later.
         *--------------------------------------------------------------------------------*/
        this->set_buffer("buffer", buffer);
    }
}

void BufferLookup::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->num_output_channels = buffer->get_num_channels();
    }
    this->Node::set_buffer(name, buffer);
}

void BufferLookup::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    for (int frame = 0; frame < num_frames; frame++)
    {
        /*--------------------------------------------------------------------------------
         * get_frame() clamps to the buffer's bounds and applies its interpolation mode.
         *--------------------------------------------------------------------------------*/
        double index = this->index->out[0][frame];
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = this->buffer->get_frame(channel, index);
        }
    }
}

}
