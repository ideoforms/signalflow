#include "signalflow/core/graph.h"
#include "signalflow/node/analysis/nearest-neighbour.h"

namespace signalflow
{

NearestNeighbour::NearestNeighbour(BufferRef buffer, NodeRef target)
    : target(target)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "nearest-neighbour";
    this->kdtree = nullptr;

    this->create_buffer("buffer", this->buffer);
    this->create_input("target", this->target);

    if (buffer)
    {
        /*--------------------------------------------------------------------------------
         * Note that buffer can initially be null (e.g, when a node is deserialised).
         *--------------------------------------------------------------------------------*/
        this->set_buffer("buffer", buffer);
    }
}

void NearestNeighbour::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->Node::set_buffer(name, buffer);

        /*--------------------------------------------------------------------------------
         * Initialise K-D tree data structure.
         * If the tree had previously been assigned, free it first.
         *--------------------------------------------------------------------------------*/
        if (this->kdtree)
        {
            delete this->kdtree;
        }
        std::vector<std::vector<float>> data;
        for (size_t i = 0; i < buffer->get_num_frames(); i++)
        {
            // must be a more efficient way to express this -- basically a transpose
            std::vector<float> item;
            for (size_t channel = 0; channel < buffer->get_num_channels(); channel++)
            {
                item.push_back(this->buffer->data[channel][i]);
            }
            data.push_back(item);
        }
        this->kdtree = new KDTree(data);

        // TODO: set num output channels to # channels in buffer
    }
}

void NearestNeighbour::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    int num_channels = this->target->get_num_output_channels();
    std::vector<float> target_value_vector;
    for (int i = 0; i < num_channels; i++)
    {
        target_value_vector.push_back(this->target->out[i][0]);
    }

    KDTreeMatch match = this->kdtree->get_nearest(target_value_vector);
    int index = match.index;
    for (auto channel = 0; channel < this->get_num_output_channels(); channel++)
    {
        for (auto frame = 0; frame < num_frames; frame++)
        {
            this->out[channel][frame] = index;
        }
    }
}

}
