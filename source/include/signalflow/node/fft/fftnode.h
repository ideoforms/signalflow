#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class FFTNode : public Node
{
public:
    FFTNode(int fft_size, int hop_size, int window_size, bool do_window);
    ~FFTNode();

    sample **magnitudes;
    sample **phases;
    int fft_size;
    int hop_size;
    int num_bins;
    int num_hops;
    int window_size;
    bool do_window;
};

class FFTOpNode : public FFTNode
{
public:
    FFTOpNode(NodeRef input = nullptr);

    virtual void set_input(std::string name, const NodeRef &node);

    NodeRef input;
};
}
