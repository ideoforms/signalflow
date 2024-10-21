#pragma once

/**--------------------------------------------------------------------------------
 * @file graph.h
 * @brief AudioGraph encapsulates the entire signal processing graph, containing
 *        Nodes, Patches, audio inputs and outputs.
 *
 *--------------------------------------------------------------------------------*/

#include "signalflow/core/config.h"
#include "signalflow/core/core.h"
#include "signalflow/node/io/output/abstract.h"
#include "signalflow/node/node.h"
#include "signalflow/patch/patch.h"

#include <sndfile.h>

namespace signalflow
{

class AudioGraphMonitor;

class AudioGraph
{
public:
    AudioGraph(AudioGraphConfig *config = nullptr, std::string output_device = "", bool start = true);
    AudioGraph(AudioGraphConfig *config, NodeRef output_device, bool start = true);
    AudioGraph(std::string config_name, std::string output_device = "", bool start = true);
    AudioGraph(std::string config_name, NodeRef output_device, bool start = true);

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
     * Remove all nodes from the graph.
     * Also removes NodeMonitors.
     *
     **--------------------------------------------------------------------------------*/
    void clear();

    /**--------------------------------------------------------------------------------
     * Terminate and free audio I/O, and terminate the shared graph.
     *
     **--------------------------------------------------------------------------------*/
    void destroy();

    /**--------------------------------------------------------------------------------
     * Run (and block the main thread) for a given number of seconds, or forever if
     * a time is not specified.
     *
     * @param time Time to run, in seconds.
     *
     *--------------------------------------------------------------------------------*/
    void wait(float time = -1.f);

    /**--------------------------------------------------------------------------------
     * Print the AudioGraph structure to stdout.
     *
     *--------------------------------------------------------------------------------*/
    void show_structure();
    std::string get_structure();
    std::string get_structure(NodeRef &root, int depth);

    /**--------------------------------------------------------------------------------
     * Get current graph status (node count, patch count, CPU usage).
     *
     *--------------------------------------------------------------------------------*/
    std::string get_status();

    /**--------------------------------------------------------------------------------
     * Print current graph status to stdout.
     * If frequency is non-zero, show status every `frequency` seconds.
     * If frequency is zero, show status once (and stop recurrently showing status).
     *
     * @param frequency Interval between prints, in seconds.
     *
     *--------------------------------------------------------------------------------*/
    void poll(float frequency = 0.0);

    /**--------------------------------------------------------------------------------
     * Perform batch (offline) processing of a given node graph.
     * This should now be deprecated in favour of the improved render functions.
     *
     * @param root The root node to begin process from.
     * @param num_frames The number of frames to render.
     * @param block_size The size of each chunk to process.
     *
     *--------------------------------------------------------------------------------*/
    // void process(const NodeRef &root, int num_frames, int block_size = SIGNALFLOW_DEFAULT_BLOCK_SIZE);

    /**--------------------------------------------------------------------------------
     * Render the entire graph.
     *  - Reset "rendered" flags
     *  - Perform a recursive render from the output node
     *  - Calculate timing and CPU usage
     *
     * Typically this is called from the audio I/O thread and does not need to be
     * called manually. To render to a buffer, use render_to_buffer.
     *
     * @param num_frames The number of frames to render.
     *
     *--------------------------------------------------------------------------------*/
    void render(int num_frames);
    void render();

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
    void render_subgraph(const NodeRef &node);

    /**--------------------------------------------------------------------------------
     * Render the entire graph to an output buffer, dividing the processing into
     * equal-sized blocks.
     *
     * @param buffer The buffer to render to. The buffer must have a channel count
     *               equal or less than that of the graph's output unit.
     *
     *--------------------------------------------------------------------------------*/
    void render_to_buffer(BufferRef buffer);

    /**--------------------------------------------------------------------------------
     * Allocate an output buffer of specified duration, and render the graph to it.
     *
     * @param duration The duration of the render, in seconds.
     *
     *--------------------------------------------------------------------------------*/
    BufferRef render_to_new_buffer(int num_frames);

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
     * Set the current audio output device.
     *
     * @param output_device The new audio output device.
     *
     *--------------------------------------------------------------------------------*/
    void set_output(NodeRef output_device);

    /**--------------------------------------------------------------------------------
     * Returns a list of the nodes currently connected to the AudioGraph's output.
     *
     * @return The output nodes.
     *
     *--------------------------------------------------------------------------------*/
    std::list<NodeRef> get_outputs();

    /**--------------------------------------------------------------------------------
     * Returns a list of available audio I/O output devices.
     *
     * @return The list of device names.
     *
     *--------------------------------------------------------------------------------*/
    static std::list<std::string> get_output_device_names(std::string backend_name = "");

    /**--------------------------------------------------------------------------------
     * Returns a list of available audio I/O input devices.
     *
     * @return The list of device names.
     *
     *--------------------------------------------------------------------------------*/
    static std::list<std::string> get_input_device_names(std::string backend_name = "");

    /**--------------------------------------------------------------------------------
     * Returns a list of available audio I/O output backends.
     *
     * @return The list of backend names.
     *
     *--------------------------------------------------------------------------------*/
    static std::list<std::string> get_backend_names();

    /**--------------------------------------------------------------------------------
     * Schedule a node for rendering without connecting the node to the graph's output.
     * This is needed for nodes such as LFOs, whose values need to be calculated
     * independently of the existence of synthesis nodes, regardless of whether
     * their output is being used.
     *
     * @param node The node.
     * @returns The same node, for consistency of interface with Patch::add_node.
     *
     *--------------------------------------------------------------------------------*/
    NodeRef add_node(NodeRef node);

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
     * @throws node_already_playing_exception if the node is already playing
     * @throws cpu_usage_above_limit_exception if CPU usage is above the acceptable limit.
     *
     *--------------------------------------------------------------------------------*/
    void play(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Query whether the specified node is currently playing (i.e., connected to
     * the graph's output).
     *
     * @param node The node to query
     * @returns true if playing, false otherwise
     *
     *--------------------------------------------------------------------------------*/
    bool is_playing(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Similar to add_node, but for patches.
     * Is a bit of a hacky workaround for the situation in which a Patch is connected
     * to a node for playback - this does not register the Patch as playing or parse it,
     * so auto_free breaks and the patch hangs around forever. Needs fixing at some point.
     *
     * @param patch The patch to add.
     * @throws cpu_usage_above_limit_exception if CPU usage is above the acceptable limit.
     *--------------------------------------------------------------------------------*/
    void add_patch(PatchRef patch);

    /**--------------------------------------------------------------------------------
     * Begin playing the specified patch, by connecting it to the graph's output.
     * This can also be written as patch->connect(graph->get_output()).
     *
     * @param patch The patch to begin playing.
     * @throws node_already_playing_exception if the node is already playing
     * @throws cpu_usage_above_limit_exception if CPU usage is above the acceptable limit.
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
     * Replace the specified node with another.
     *
     * @param node The node to stop.
     * @param other The replacement node.
     *
     *--------------------------------------------------------------------------------*/
    void replace(NodeRef node, NodeRef other);

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
     * Start recording the graph's output to a named file.
     *
     * @param filename The file to write to.
     * @param num_channels The number of channels to record. If not specified,
     *                     uses the same number of channels as the graph's
     *                     output device.
     *
     *--------------------------------------------------------------------------------*/
    void start_recording(const std::string &filename, int num_channels = 0);

    /**--------------------------------------------------------------------------------
     * Stop recording the graph's output.
     *
     *--------------------------------------------------------------------------------*/
    void stop_recording();

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
     * Get the audio output buffer size, in frames.
     * This returns the actual buffer size used by the audio hardware.
     * To query the requested buffer size, call graph.config.get_output_buffer_size()
     *
     * @return The output buffer size, in frames.
     *
     *--------------------------------------------------------------------------------*/
    int get_output_buffer_size();

    /**--------------------------------------------------------------------------------
     * Query the number of output channels, which in turn queries the
     * AudioOut_Abstract subclass that the AudioGraph is connected to.
     *
     * @return The number of channels.
     *
     *--------------------------------------------------------------------------------*/
    int get_num_output_channels();

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
     * Query the amount of RAM used by Buffer storage.
     *
     * @return The number of bytes stored in RAM.
     *
     *--------------------------------------------------------------------------------*/
    size_t get_memory_usage();

    /**--------------------------------------------------------------------------------
     * Get the current graph config.
     *
     * @return The config.
     *
     *--------------------------------------------------------------------------------*/
    AudioGraphConfig &get_config();

    static AudioGraph *get_shared_graph();

    bool has_raised_audio_thread_error();
    void raise_audio_thread_error();

protected:
    void init();
    void register_memory_alloc(size_t num_bytes);
    void register_memory_dealloc(size_t num_bytes);

private:
    std::set<NodeRef> scheduled_nodes;
    std::set<NodeRef> nodes_to_remove;
    std::set<NodeRef> scheduled_nodes_to_remove;
    std::set<std::pair<NodeRef, NodeRef>> nodes_to_replace;
    std::set<PatchRef> patches;
    std::set<Patch *> patches_to_remove;

    void show_structure(NodeRef &root, int depth);
    AudioGraphMonitor *monitor;
    int sample_rate;
    int node_count;
    int _node_count_tmp;
    float cpu_usage;
    float cpu_usage_smoothing;
    float peak_level;
    size_t memory_usage;
    bool raised_audio_thread_error = false;

    NodeRef output = nullptr;
    AudioGraphConfig config;

    SNDFILE *recording_fd;
    float *recording_buffer;
    int recording_num_channels;

    friend class Buffer;
    friend class FFTBuffer;
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
