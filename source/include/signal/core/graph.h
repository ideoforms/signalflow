#pragma once

#include "signal/node/node.h"
#include "signal/synth/synth.h"

namespace libsignal
{

class AudioGraph
{
public:
    AudioGraph();
    virtual ~AudioGraph();

    /**------------------------------------------------------------------------
     * Begin audio I/O.
     *
     *------------------------------------------------------------------------*/
    void start();

    /**------------------------------------------------------------------------
     * Stop audio I/O.
     *
     *------------------------------------------------------------------------*/
    void stop();

    /**------------------------------------------------------------------------
     * Run forever.
     *
     *------------------------------------------------------------------------*/
    void wait();

    /**------------------------------------------------------------------------
     * Print graph structure.
     *
     *------------------------------------------------------------------------*/
    void print();

    /**------------------------------------------------------------------------
     * Perform batch (offline) processing of a given node graph.
     *
     *------------------------------------------------------------------------*/
    void process(const NodeRef &root, int num_frames, int block_size = SIGNAL_DEFAULT_BLOCK_SIZE);

    void pull_input(const NodeRef &node, int num_frames);
    void pull_input(int num_frames);

    NodeRef get_output();

    /**------------------------------------------------------------------------
     * TODO Should use polymorphism and a common interface
     *
     *------------------------------------------------------------------------*/
    void add_output(SynthRef synth);
    void add_output(NodeRef node);

    void remove_output(SynthRef synth);
    void remove_output(NodeRef node);

    NodeRef add_node(Node *node);

    NodeRef input = nullptr;
    NodeRef output = nullptr;

    int sample_rate;
    int node_count;

private:
    std::set<Node *> processed_nodes;
    std::set<NodeRef> output_nodes_to_remove;

    void print(NodeRef &root, int depth);
};

class AudioGraphRef : public std::shared_ptr<AudioGraph>
{
public:
    using std::shared_ptr<AudioGraph>::shared_ptr;

    AudioGraphRef()
        : std::shared_ptr<AudioGraph>(nullptr) {}
    AudioGraphRef(AudioGraph *ptr)
        : std::shared_ptr<AudioGraph>(ptr) {}
};

}
