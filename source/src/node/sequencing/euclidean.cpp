#include "signalflow/node/sequencing/euclidean.h"

namespace signalflow
{

void _print(std::vector<std::vector<int>> sequence)
{
    for (auto i : sequence)
    {
        for (auto j : i)
        {
            std::cout << j << ", ";
        }
        std::cout << " | ";
    }
    std::cout << std::endl;
}

std::vector<std::vector<std::vector<int>>> _partition(std::vector<std::vector<int>> sequence)
{
    std::vector<std::vector<std::vector<int>>> partitions(2);
    bool found_partition = false;
    for (int i = 0; i < sequence.size(); i++)
    {
        if (i == 0 || !found_partition)
        {
            if (i > 0 && sequence[i] != sequence[i - 1])
            {
                partitions[1].push_back(sequence[i]);
                found_partition = true;
            }
            else
            {
                partitions[0].push_back(sequence[i]);
            }
        }
        else
        {
            partitions[1].push_back(sequence[i]);
        }
    }
    return partitions;
}

std::vector<std::vector<int>> _interleave(std::vector<std::vector<int>> sequenceA, std::vector<std::vector<int>> sequenceB)
{
    int num_arrays = (sequenceA.size() > sequenceB.size()) ? sequenceA.size() : sequenceB.size();
    std::vector<std::vector<int>> rv(num_arrays);
    for (int i = 0; i < num_arrays; i++)
    {
        if (i < sequenceA.size())
        {
            rv[i].insert(rv[i].end(), sequenceA[i].begin(), sequenceA[i].end());
        }
        if (i < sequenceB.size())
        {
            rv[i].insert(rv[i].end(), sequenceB[i].begin(), sequenceB[i].end());
        }
    }
    return rv;
}

std::vector<int> _flatten(std::vector<std::vector<int>> sequence)
{
    std::vector<int> rv;
    for (auto subsequence : sequence)
    {
        rv.insert(rv.end(), subsequence.begin(), subsequence.end());
    }
    return rv;
}

std::vector<int> _generate(int sequence_length, int num_events)
{
    std::vector<std::vector<int>> sequence(sequence_length, std::vector<int>(1));
    for (int i = 0; i < num_events; i++)
    {
        sequence[i][0] = 1;
    }
    std::vector<std::vector<std::vector<int>>> partitions = _partition(sequence);
    while (true)
    {
        if (partitions[1].size() <= 1)
        {
            break;
        }
        else
        {
            sequence = _interleave(partitions[0], partitions[1]);
            partitions = _partition(sequence);
        }
    }
    std::vector<int> sequence_flattened = _flatten(sequence);
    return sequence_flattened;
}

Euclidean::Euclidean(NodeRef clock, NodeRef sequence_length, NodeRef num_events)
    : Node(), clock(clock), sequence_length(sequence_length), num_events(num_events)
{
    this->name = "counter";

    this->create_input("clock", this->clock);
    this->create_input("sequence_length", this->sequence_length);
    this->create_input("num_events", this->num_events);

    this->position = -1;
    this->sequence_length_cur = 0;
    this->num_events_cur = 0;
    this->events = std::vector<int>(256, 1);

    this->alloc();
}

void Euclidean::_recalculate(int sequence_length, int num_events)
{
    this->sequence_length_cur = sequence_length;
    this->num_events_cur = num_events;
    this->events = _generate(sequence_length, num_events);
}

void Euclidean::trigger(std::string name, float value)
{
}

void Euclidean::process(Buffer &out, int num_frames)
{
    if ((int) this->sequence_length->out[0][0] != this->sequence_length_cur || (int) this->num_events->out[0][0] != this->num_events_cur)
    {
        this->_recalculate((int) this->sequence_length->out[0][0],
                           (int) this->num_events->out[0][0]);
    }

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool rv = SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
            if (rv)
            {
                this->position = (this->position + 1) % this->sequence_length_cur;
                this->out[channel][frame] = this->events[this->position];
            }
            else
            {
                this->out[channel][frame] = 0.0;
            }
        }
    }
}

}
