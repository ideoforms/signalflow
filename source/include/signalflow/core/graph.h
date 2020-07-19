#pragma once

/**--------------------------------------------------------------------------------
 * @file graph.h
 * @brief AudioGraph encapsulates the entire signal processing graph, containing
 *        Nodes, Patches, audio inputs and outputs.
 *
 *--------------------------------------------------------------------------------*/

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

    /**--------------------------------------------------------------------------------
     * Begin audio I/O.
     *
     **--------------------------------------------------------------------------------*/
    void start();

    /**--------------------------------------------------------------------------------
     * Stop audio I/O.
     *
     *--------------------------------------------------------------------------------*/
    void stop();

    /**--------------------------------------------------------------------------------
     * Run (and block the main thread) for a given number of seconds, or forever if
     * not specified.
     *
     * @param time Time to run, in seconds.
     *
     *--------------------------------------------------------------------------------*/
    void wait(float time = 0.0f);

    /**--------------------------------------------------------------------------------
     * Print the AudioGraph structure to stdout.
     *
     *--------------------------------------------------------------------------------*/
    void show_structure();

    /**--------------------------------------------------------------------------------
     * Print current graph status (node count, patch count, CPU usage) to stdout.
     * If frequency is non-zero, show status every `frequency` seconds.
     *
     * @param frequency Interval between prints, in seconds.
     *
     *--------------------------------------------------------------------------------*/
    void show_status(float frequency = 0.0);

    /**--------------------------------------------------------------------------------
     * Perform batch (offline) processing of a given node graph.
     *
     * @param root The root node to begin process from.
     * @param num_frames The number of frames to render.
     * @param block_size The size of each chunk to process.
     *
     *--------------------------------------------------------------------------------*/
    // void process(const NodeRef &root, int num_frames, int block_size = SIGNAL_DEFAULT_BLOCK_SIZE);

    /**--------------------------------------------------------------------------------
     * Render the entire graph.
     *  - Resets "rendered" flags
     *  - Performs a recursive render from the output node
     *  - Calculates timing and CPU usage
     * Typically is called from
     *
     * @param num_frames The number of frames to render.
     *
     *--------------------------------------------------------------------------------*/
    void render(int num_frames);
    void render_to_buffer(BufferRef buffer, int block_size = SIGNAL_DEFAULT_BLOCK_SIZE);

    void traverse_graph(const NodeRef &node, int num_frames);
    void reset_graph();
    void reset_graph(NodeRef root);

    NodeRef get_output();

    /**--------------------------------------------------------------------------------
     * TODO Should use polymorphism and a common interface
     *
     *--------------------------------------------------------------------------------*/
    void add_output(PatchRef patch);
    void add_output(NodeRef node);

    void add_node(NodeRef node);

    void remove_output(Patch *patch);
    void remove_output(PatchRef patch);
    void remove_output(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Get/set audio sample rate
     *--------------------------------------------------------------------------------*/
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
