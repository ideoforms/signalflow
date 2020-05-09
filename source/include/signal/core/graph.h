#pragma once

#include "signal/node/node.h"
#include "signal/synth/synth.h"

namespace libsignal
{

class AudioGraphMonitor;

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
     * Poll graph state every so often.
     *
     *------------------------------------------------------------------------*/
    void poll(float frequency);

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

    void remove_output(Synth *synth);
    void remove_output(SynthRef synth);
    void remove_output(NodeRef node);

    int get_node_count();
    int get_synth_count();
    float get_cpu_usage();

    NodeRef input = nullptr;
    NodeRef output = nullptr;

    int sample_rate;

private:
    std::set<Node *> processed_nodes;
    std::set<NodeRef> nodes_to_remove;
    std::set<SynthRef> synths;
    std::set<Synth *> synths_to_remove;

    void print(NodeRef &root, int depth);
    AudioGraphMonitor *monitor;
    int node_count;
    float cpu_usage;
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
