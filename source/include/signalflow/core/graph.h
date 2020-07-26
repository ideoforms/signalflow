#pragma once

/**--------------------------------------------------------------------------------
 * @file graph.h
 * @brief AudioGraph encapsulates the entire signal processing graph, containing
 *        Nodes, Patches, audio inputs and outputs.
 *
 *--------------------------------------------------------------------------------*/

#include "signalflow/core/config.h"
#include "signalflow/node/io/output/abstract.h"
#include "signalflow/node/node.h"
#include "signalflow/patch/patch.h"

namespace signalflow
{

class AudioGraphMonitor;

class AudioGraph
{
public:
    AudioGraph(SignalFlowConfig *config = nullptr,
               NodeRef output_device = nullptr);

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
     * a time is not specified.
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
     * This should now be deprecated in favour of the improved render functions.
     *
     * @param root The root node to begin process from.
     * @param num_frames The number of frames to render.
     * @param block_size The size of each chunk to process.
     *
     *--------------------------------------------------------------------------------*/
    // void process(const NodeRef &root, int num_frames, int block_size = SIGNAL_DEFAULT_BLOCK_SIZE);

    /**--------------------------------------------------------------------------------
     * Render the entire graph.
     *  - Reset "rendered" flags
     *  - Perform a recursive render from the output node
     *  - Calculate timing and CPU usage
     *
     * Typically this is called from the audio I/O thread and does not need to be
     * called manually. To render to a buffer, use render_to_buffer
     *
     * @param num_frames The number of frames to render.
     *
     *--------------------------------------------------------------------------------*/
    void render(int num_frames);

    /**--------------------------------------------------------------------------------
     * Perform a recursive render from the specified node.
     *
     * Note that this does not reset rendered flags. If you want to repeatedly
     * render the same subgraph, call reset_subgraph between calls.
     *
     * Typically this does not need to be called manually.
     *
     * @param node The root node.
     * @param num_frames The number of frames to render.
     *
     *--------------------------------------------------------------------------------*/
    void render_subgraph(const NodeRef &node, int num_frames);

    /**--------------------------------------------------------------------------------
     * Render the entire graph to an output buffer, dividing the processing into
     * equal-sized blocks.
     *
     * @param buffer The buffer to render to. The buffer must have a channel count
     *               equal or less than that of the graph's output unit.
     * @param block_size The size of each block to process, in frames.
     *
     *--------------------------------------------------------------------------------*/
    void render_to_buffer(BufferRef buffer, int block_size = SIGNAL_DEFAULT_BLOCK_SIZE);

    /**--------------------------------------------------------------------------------
     * Reset the audio graph:
     *  - do garbage collection on any nodes/patches that are scheduled to be removed
     *  - clear rendered flags across the whole graph
     *
     * This is done automatically in each call to render() so does not typically
     * need to be called manually.
     *
     *--------------------------------------------------------------------------------*/
    void reset_graph();

    /**--------------------------------------------------------------------------------
     * Clear the rendered flag across nodes beneath the specified root.
     * Does not typically need to be called manually.
     *
     * @param root The root of the subgraph to reset.
     *             All inputs are reset, recursively.
     *
     *--------------------------------------------------------------------------------*/
    void reset_subgraph(NodeRef root);

    /**--------------------------------------------------------------------------------
     * Query the audio graph's output node (typically an instance of an
     * AudioOut subclass).
     *
     * @return The output node.
     *
     *--------------------------------------------------------------------------------*/
    NodeRef get_output();

    /**--------------------------------------------------------------------------------
     * Schedule a node for rendering without connecting the node to the graph's output.
     * This is needed for nodes such as LFOs, whose values need to be calculated
     * independently of the existence of synthesis nodes, regardless of whether
     * their output is being used.
     *
     * @param node The node.
     *
     *--------------------------------------------------------------------------------*/
    void add_node(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Remove a node from the graph's playback schedule.
     *
     * @param node The node.
     *
     *--------------------------------------------------------------------------------*/
    void remove_node(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Begin playing the specified node, by connecting it to the graph's output.
     * This can also be written as node->connect(graph->get_output()).
     *
     * @param node The node to begin playing.
     *
     *--------------------------------------------------------------------------------*/
    void play(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Begin playing the specified patch, by connecting it to the graph's output.
     * This can also be written as patch->connect(graph->get_output()).
     *
     * @param patch The patch to begin playing.
     *
     *--------------------------------------------------------------------------------*/
    void play(PatchRef patch);

    /**--------------------------------------------------------------------------------
     * Disconnect a node from the graph's output.
     *
     * @param node The node to stop.
     *
     *--------------------------------------------------------------------------------*/
    void stop(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Disconnect a patch from the graph's output.
     *
     * @param patch The patch to stop.
     *
     *--------------------------------------------------------------------------------*/
    void stop(Patch *patch);

    /**--------------------------------------------------------------------------------
     * Disconnect a patch from the graph's output.
     *
     * @param patch The patch to stop.
     *
     *--------------------------------------------------------------------------------*/
    void stop(PatchRef patch);

    /**--------------------------------------------------------------------------------
     * Get audio sample rate.
     *
     * @return The sample rate, in Hz.
     *
     *--------------------------------------------------------------------------------*/
    int get_sample_rate();

    /**--------------------------------------------------------------------------------
     * Set the audio sample rate.
     * Note that this will not alter the sample rate of the audio I/O hardware, if
     * the audio graph is connected to system audio. A mismatch between the audio
     * I/O and graph sample rates will result in time-distorted playback.
     *
     * @param sample_rate The sample rate, in Hz.
     *
     *--------------------------------------------------------------------------------*/
    void set_sample_rate(int sample_rate);

    /**--------------------------------------------------------------------------------
     * Get the audio output buffer size.
     *
     * @return The output buffer size, in frames.
     *
     *--------------------------------------------------------------------------------*/
    int get_output_buffer_size();

    /**--------------------------------------------------------------------------------
     * Query the number of nodes in the audio graph.
     * This includes all of the nodes in every patch.
     *
     * @return The number of nodes.
     *
     *--------------------------------------------------------------------------------*/
    int get_node_count();

    /**--------------------------------------------------------------------------------
      * Query the number of patches in the audio graph.
      *
      * @return The number of patches.
      *
      *--------------------------------------------------------------------------------*/
    int get_patch_count();

    /**--------------------------------------------------------------------------------
      * Query the CPU usage.
      *
      * @return The proportion of clock cycles available vs the clock
      *         cycles consumed in the last audio I/O callback, between 0..1.
      *
      *--------------------------------------------------------------------------------*/
    float get_cpu_usage();

    /**--------------------------------------------------------------------------------
      * Get the current graph config.
      *
      * @return The config.
      *
      *--------------------------------------------------------------------------------*/
    SignalFlowConfig &get_config();

private:
    std::set<NodeRef> scheduled_nodes;
    std::set<NodeRef> nodes_to_remove;
    std::set<PatchRef> patches;
    std::set<Patch *> patches_to_remove;

    void show_structure(NodeRef &root, int depth);
    AudioGraphMonitor *monitor;
    int sample_rate;
    int node_count;
    int _node_count_tmp;
    float cpu_usage;

    NodeRef input = nullptr;
    NodeRef output = nullptr;
    SignalFlowConfig config;
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
