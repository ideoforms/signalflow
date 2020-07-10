#pragma once

#include "signalflow/node/node.h"
#include "signalflow/patch/patch.h"

namespace signalflow
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
    void wait(float time = 0.0f);

    /**------------------------------------------------------------------------
     * Print graph structure.
     *
     *------------------------------------------------------------------------*/
    void show_structure();

    /**------------------------------------------------------------------------
     * Print current graph status (node count, patch count, CPU usage).
     * If frequency is n on-zero, show status every `frequency` seconds.
     *
     *------------------------------------------------------------------------*/
    void show_status(float frequency = 0.0);

    /**------------------------------------------------------------------------
     * Perform batch (offline) processing of a given node graph.
     *
     *------------------------------------------------------------------------*/
    void process(const NodeRef &root, int num_frames, int block_size = SIGNAL_DEFAULT_BLOCK_SIZE);

    void render(int num_frames);
    void render_to_buffer(BufferRef buffer, int block_size = SIGNAL_DEFAULT_BLOCK_SIZE);

    void traverse_graph(const NodeRef &node, int num_frames);
    void reset_graph();
    void reset_graph(NodeRef root);

    NodeRef get_output();

    /**------------------------------------------------------------------------
     * TODO Should use polymorphism and a common interface
     *
     *------------------------------------------------------------------------*/
    void add_output(PatchRef patch);
    void add_output(NodeRef node);

    void remove_output(Patch *patch);
    void remove_output(PatchRef patch);
    void remove_output(NodeRef node);

    /**------------------------------------------------------------------------
     * Get/set audio sample rate
     *------------------------------------------------------------------------*/
    int get_sample_rate();
    void set_sample_rate(int sample_rate);

    int get_node_count();
    int get_patch_count();
    float get_cpu_usage();

    NodeRef input = nullptr;
    NodeRef output = nullptr;

private:
    std::set<NodeRef> nodes_to_remove;
    std::set<PatchRef> patches;
    std::set<Patch *> patches_to_remove;

    void show_structure(NodeRef &root, int depth);
    AudioGraphMonitor *monitor;
    int sample_rate;
    int node_count;
    int _node_count_tmp;
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
