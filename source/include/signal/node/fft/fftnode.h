#pragma once

#include "signal/node/node.h"

namespace libsignal
{
class FFTNode : public Node
{
public:
    FFTNode(int fft_size, int hop_size);
    ~FFTNode();

    sample **magnitudes;
    sample **phases;
    int fft_size;
    int hop_size;
    int num_bins;
    int num_hops;
};

class FFTOpNode : public FFTNode
{
public:
    FFTOpNode(NodeRef input = nullptr);

    virtual void set_input(std::string name, const NodeRef &node);

    NodeRef input;
};
}
