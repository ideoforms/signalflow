#include "signalflow/core/core.h"
#include "signalflow/core/graph-monitor.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/io/output/abstract.h"
#include "signalflow/node/io/output/dummy.h"
#include "signalflow/node/io/output/ios.h"
#include "signalflow/node/io/output/soundio.h"
#include "signalflow/node/node.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/patch/patch.h"

#include <iomanip>
#include <limits.h>
#include <math.h>
#include <sstream>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace signalflow
{

AudioGraph *shared_graph = nullptr;

AudioGraph *AudioGraph::get_shared_graph()
{
    return shared_graph;
}

AudioGraph::AudioGraph(AudioGraphConfig *config, std::string output_device, bool start)
{
    if (shared_graph)
    {
        /*--------------------------------------------------------------------------------
         * TODO: At some point, it would be nice to allow the creation of multiple
         *       AudioGraphs, or recreating the shared AudioGraph (perhaps given some
         *       config option). However, this may have unforeseen circumstances and
         *       would need memory/CPU to be correctly cleaned up, which does not happen
         *       right now.
         *--------------------------------------------------------------------------------*/
        throw graph_already_created_exception();
    }
    shared_graph = this;

    if (config)
    {
        this->config = *config;
    }

    if (output_device == "dummy")
    {
        this->output = new AudioOut_Dummy();
    }
    else if (output_device == "")
    {
        this->output = new AudioOut(this->config.get_output_backend_name(),
                                    this->config.get_output_device_name(),
                                    this->config.get_sample_rate(),
                                    this->config.get_output_buffer_size());
        if (!this->output)
        {
            throw std::runtime_error("AudioGraph: Couldn't find audio output device");
        }
    }
    else
    {
        throw std::runtime_error("AudioGraph: Invalid output device name: " + output_device);
    }

    this->init();

    if (start)
    {
        this->start();
    }
}

AudioGraph::AudioGraph(AudioGraphConfig *config, NodeRef output_device, bool start)
{
    if (shared_graph)
    {
        throw graph_already_created_exception();
    }
    shared_graph = this;

    if (config)
    {
        this->config = *config;
    }

    if (output_device)
    {
        this->output = output_device;
    }
    else
    {
        this->output = new AudioOut(this->config.get_output_backend_name(),
                                    this->config.get_output_device_name(),
                                    this->config.get_sample_rate(),
                                    this->config.get_output_buffer_size());
        if (!this->output)
        {
            throw std::runtime_error("AudioGraph: Couldn't find audio output device");
        }
    }

    this->init();

    if (start)
    {
        this->start();
    }
}

void AudioGraph::init()
{
    signalflow_init();

    AudioOut_Abstract *audio_out = (AudioOut_Abstract *) this->output.get();

    if (audio_out->get_sample_rate() == 0)
    {
        throw std::runtime_error("AudioGraph: Audio output device has zero sample rate");
    }

    this->sample_rate = audio_out->get_sample_rate();
    this->node_count = 0;
    this->_node_count_tmp = 0;

    /*--------------------------------------------------------------------------------
     * Monitor CPU usage, taking an exponential moving average to reduce the impact
     * of noise and spikes.
     *--------------------------------------------------------------------------------*/
    this->cpu_usage_smoothing = 0.95;
    this->cpu_usage = 0.0;

    this->memory_usage = 0;

    this->monitor = nullptr;

    this->recording_fd = NULL;
    this->recording_num_channels = 0;
    this->recording_buffer = (float *) calloc(SIGNALFLOW_DEFAULT_BLOCK_SIZE * SIGNALFLOW_MAX_CHANNELS, sizeof(float));
}

void AudioGraph::start()
{
    AudioOut_Abstract *audio_out = (AudioOut_Abstract *) this->output.get();
    audio_out->start();

    if (this->config.get_auto_record())
    {
        std::time_t now = std::time(nullptr);
        char timestamp[100];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d-%H%M%S", std::localtime(&now));
        std::string timestamp_str(timestamp);
        std::string recordings_dir = SIGNALFLOW_USER_DIR + "/recordings";
        std::string recording_filename = recordings_dir + "/signalflow-" + timestamp_str + ".wav";

        // TODO: This is all very POSIX-specific and won't work on Windows
        struct stat st;
        if (stat(SIGNALFLOW_USER_DIR.c_str(), &st) == -1)
        {
            int rv = mkdir(SIGNALFLOW_USER_DIR.c_str(), 0755);
            if (rv != 0)
            {
                throw std::runtime_error("AudioGraph: Failed creating user directory for auto_record (" + SIGNALFLOW_USER_DIR + ")");
            }
        }
        if (stat(recordings_dir.c_str(), &st) == -1)
        {
            int rv = mkdir(recordings_dir.c_str(), 0755);
            if (rv != 0)
            {
                throw std::runtime_error("AudioGraph: Failed creating recordings directory for auto_record (" + recordings_dir + ")");
            }
        }
        this->start_recording(recording_filename, this->output->get_num_input_channels());
    }
}

void AudioGraph::stop()
{
    AudioOut_Abstract *audioout = (AudioOut_Abstract *) this->output.get();
    audioout->stop();

    if (this->config.get_auto_record())
    {
        this->stop_recording();
    }
}

bool AudioGraph::has_raised_audio_thread_error()
{
    return this->raised_audio_thread_error;
}

void AudioGraph::raise_audio_thread_error()
{
    this->raised_audio_thread_error = true;
}

void AudioGraph::clear()
{
    AudioOut_Abstract *audioout = (AudioOut_Abstract *) this->output.get();
    auto inputs = audioout->get_inputs();
    for (auto input : inputs)
    {
        this->nodes_to_remove.insert(input);
    }
    for (auto node : this->scheduled_nodes)
    {
        this->scheduled_nodes_to_remove.insert(node);
    }

    patches.clear();

    this->node_count = 0;
}

void AudioGraph::destroy()
{
    // Clear output when destroyed to ensure that multiply destroy() calls won't crash
    if (this->output)
    {
        AudioOut_Abstract *audioout = (AudioOut_Abstract *) this->output.get();
        if (audioout)
        {
            audioout->destroy();
        }
        if (shared_graph == this)
        {
            shared_graph = nullptr;
        }
        this->output = nullptr;
    }
}

AudioGraph::~AudioGraph()
{
    // When the AudioGraph is deallocated (which happens automatically at the end of a Python script),
    // ensure it is destroyed.
    this->destroy();
}

void AudioGraph::wait(float time)
{
    long time_elapsed_ms = 0;
    long time_to_wait;
    if (time == -1)
    {
        time_to_wait = std::numeric_limits<long>::max();
    }
    else
    {
        time_to_wait = (long) (1000.0f * time);
    }
    while (time_elapsed_ms < time_to_wait && !this->has_raised_audio_thread_error())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        time_elapsed_ms += 10;
    }
}

void AudioGraph::render_subgraph(const NodeRef &node) { this->render_subgraph(node, this->get_output_buffer_size()); }

void AudioGraph::render_subgraph(const NodeRef &node, int num_frames)
{
    /*------------------------------------------------------------------------
     * TODO: Should be able to render subgraph to a buffer of indefinite
     *       length. Need to add block-by-block processing
     *       (as per render_to_buffer)
     *-----------------------------------------------------------------------*/

    /*------------------------------------------------------------------------
     * If this node has already been processed this timestep, return.
     *-----------------------------------------------------------------------*/
    if (node->has_rendered)
    {
        return;
    }

    if (!(node->get_inputs().size() > 0 || node->get_name() == "constant" || node->get_name() == "audioout"
          || node->get_name() == "audioin"))
    {
        signalflow_debug("Node %s has no registered inputs", node->get_name().c_str());
    }

    /*------------------------------------------------------------------------
     * Pull our inputs before we generate our own outputs.
     *-----------------------------------------------------------------------*/
    for (auto input : node->get_inputs())
    {
        NodeRef input_node = *(input.second);
        if (input_node)
        {
            this->render_subgraph(input_node, num_frames);

            /*------------------------------------------------------------------------
             * Automatic input upmix.
             *
             * If the input node produces less channels than demanded, automatically
             * up-mix its output by replicating the existing channels. This allows
             * operations between multi-channel and mono-channel inputs to work
             * seamlessly without any additional implementation within the node
             * itself (for example, Multiply(new SineOscillator(440), new StereoPanner(...)))
             *
             * A few nodes must prevent automatic input up-mixing from happening.
             * These include ChannelArray and AudioOut.
             *
             * Some partially-initialised nodes (e.g. BufferPlayer with a not-yet-
             * populated Buffer) will have num_output_channels == 0. Don't try to
             * upmix a void output.
             *-----------------------------------------------------------------------*/
            if (input_node->get_num_output_channels() < node->get_num_input_channels() && !node->no_input_upmix
                && input_node->get_num_output_channels() > 0)
            {
                signalflow_debug("Upmixing %s (%s wants %d channels, %s only produces %d)",
                                 input_node->get_name().c_str(), node->get_name().c_str(),
                                 node->get_num_input_channels(), input_node->get_name().c_str(),
                                 input_node->get_num_output_channels());

                /*------------------------------------------------------------------------
                 * Ensure the input node's output buffer re-allocation has been done.
                 * Reallocation for inputs is automatically done in Node::update_channels.
                 *-----------------------------------------------------------------------*/
                if (input_node->get_num_output_channels_allocated() < node->get_num_input_channels())
                {
                    throw std::runtime_error("Input node does not have enough buffers allocated (need "
                                             + std::to_string(node->get_num_input_channels()) + ", got "
                                             + std::to_string(input_node->get_num_output_channels_allocated()));
                }

                /*------------------------------------------------------------------------
                 * If we generate 2 channels but have 6 channels demanded, repeat
                 * them: [ 0, 1, 0, 1, 0, 1 ]
                 *-----------------------------------------------------------------------*/
                for (int out_channel_index = input_node->get_num_output_channels();
                     out_channel_index < node->get_num_input_channels(); out_channel_index++)
                {
                    int in_channel_index = out_channel_index % input_node->get_num_output_channels();
                    memcpy(input_node->out[out_channel_index], input_node->out[in_channel_index],
                           num_frames * sizeof(sample));
                }
            }
        }
    }

    node->_process(node->out, num_frames);

    if (node->get_name() != "constant" && node->get_name().substr(0, 8) != "audioout")
    {
        /*--------------------------------------------------------------------------------
         *
         *--------------------------------------------------------------------------------*/
        node->has_rendered = true;
        this->_node_count_tmp++;
    }
}

void AudioGraph::reset_graph()
{
    for (auto pair : nodes_to_replace)
    {
        AudioOut_Abstract *output = (AudioOut_Abstract *) this->output.get();
        output->replace_input(pair.first, pair.second);

        // needed to ensure stale samples don't get delivered next time around
        // as render_subgraph won't recurse to reset these nodes
        this->reset_subgraph(pair.first);
    }
    nodes_to_replace.clear();

    /*--------------------------------------------------------------------------------
     * Disconnect any nodes and patches that are scheduled to be removed.
     *--------------------------------------------------------------------------------*/
    for (auto node : nodes_to_remove)
    {
        /*--------------------------------------------------------------------------------
         * Stop any monitoring running on the node.
         *--------------------------------------------------------------------------------*/
        node->poll(0);

        while (node->outputs.size() > 0)
        {
            auto output = *(node->outputs.begin());
            Node *target = output.first;
            std::string name = output.second;

            if (target->has_variable_inputs)
            {
                target->remove_input(node);
            }
            else
            {
                /*------------------------------------------------------------------------
                 * All of this is a slightly gross way to ensure that Patch inputs
                 * are kept in sync correctly when a Patch's input node is removed.
                 * This aspect of Patch needs redesigning at some point...
                 *------------------------------------------------------------------------*/
                NodeRef zero = new Constant(0.0);
                target->set_input(name, zero);
                if (target->get_patch())
                {
                    Patch *patch = target->get_patch();
                    for (auto input : patch->get_inputs())
                    {
                        auto input_name = input.first;
                        auto input_node = input.second;
                        if (target == input_node.get())
                        {
                            patch->set_input(input_name, zero);
                        }
                    }
                }
            }
        }
    }
    nodes_to_remove.clear();

    for (auto node : this->scheduled_nodes_to_remove)
    {
        node->poll(0);
        this->scheduled_nodes.erase(node);
    }

    /*------------------------------------------------------------------------
     * Avoid segfaults if another thread modifies patches_to_remove
     * (with patch.stop()) while the audio thread is iterating over it.
     *-----------------------------------------------------------------------*/
    std::set<Patch *> patches_to_remove_copy = patches_to_remove;
    this->patches_to_remove.clear();
    for (auto patch : patches_to_remove_copy)
    {
        for (auto patchref : patches)
        {
            if (patchref.get() == patch)
            {
                patches.erase(patchref);
                break;
            }
        }
    }

    /*------------------------------------------------------------------------
     * Clear the record of processed nodes.
     *-----------------------------------------------------------------------*/
    this->_node_count_tmp = 0;
    this->reset_subgraph(this->output);
    for (auto node : this->scheduled_nodes)
    {
        this->reset_subgraph(node);
    }
}

void AudioGraph::reset_subgraph(NodeRef node)
{
    node->has_rendered = false;
    for (auto input : node->get_inputs())
    {
        NodeRef input_node = *(input.second);
        if (input_node && input_node->has_rendered)
        {
            this->reset_subgraph(input_node);
        }
    }
}

void AudioGraph::render() { this->render(this->get_output_buffer_size()); }

void AudioGraph::render(int num_frames)
{
    /*------------------------------------------------------------------------
     * Timestamp the start of processing to measure CPU usage.
     *-----------------------------------------------------------------------*/
    double t0 = signalflow_timestamp();

    this->reset_graph();
    this->render_subgraph(this->output, num_frames);
    for (auto node : this->scheduled_nodes)
    {
        this->render_subgraph(node, num_frames);
    }
    this->node_count = this->_node_count_tmp;
    signalflow_debug("AudioGraph: pull %d frames, %d nodes", num_frames, this->node_count);

    if (this->recording_fd)
    {
        /*------------------------------------------------------------------------
         * If recording, interleave (for libsndfile format) and write to file.
         * TODO: This breaks the cardinal rule of doing file I/O in the audio
         *       thread. Refactor to use threading and ringbuffers.
         *-----------------------------------------------------------------------*/
        for (int channel_index = 0; channel_index < this->recording_num_channels; channel_index++)
        {
            for (int frame_index = 0; frame_index < num_frames; frame_index++)
            {
                this->recording_buffer[frame_index * this->recording_num_channels + channel_index]
                    = this->output->out[channel_index][frame_index];
            }
        }
        sf_writef_float(this->recording_fd, this->recording_buffer, num_frames);
    }

    this->peak_level = 0.0;
    for (int channel = 0; channel < this->output->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample level_abs = abs(this->output->out[channel][frame]);
            if (level_abs > this->peak_level)
            {
                this->peak_level = level_abs;
            }
        }
    }

    /*------------------------------------------------------------------------
     * Calculate CPU usage (approximately) by measuring the % of time
     * within the audio I/O callback that was used for processing.
     *-----------------------------------------------------------------------*/
    double t1 = signalflow_timestamp();
    double dt = t1 - t0;
    double t_max = (double) num_frames / this->sample_rate;
    float cpu_usage = dt / t_max;
    if (cpu_usage > 1.0)
    {
        std::cerr << "Warning: buffer overrun?" << std::endl;
    }
    this->cpu_usage = (1.0 - this->cpu_usage_smoothing) * cpu_usage + this->cpu_usage_smoothing * this->cpu_usage;
}

void AudioGraph::render_to_buffer(BufferRef buffer)
{
    // TODO get_num_output_channels()
    int channel_count = buffer->get_num_channels();
    int block_size = this->get_output_buffer_size();
    if (channel_count > this->output->num_input_channels)
    {
        throw std::runtime_error("Buffer cannot have more channels than the audio graph ("
                                 + std::to_string(channel_count)
                                 + " != " + std::to_string(this->output->num_input_channels) + ")");
    }
    int block_count = ceilf((float) buffer->get_num_frames() / block_size);

    for (int block_index = 0; block_index < block_count; block_index++)
    {
        int block_frames = block_size;
        if (block_index == block_count - 1 && buffer->get_num_frames() % block_size > 0)
        {
            block_frames = buffer->get_num_frames() % block_size;
        }
        this->render(block_frames);
        for (int channel_index = 0; channel_index < channel_count; channel_index++)
        {
            memcpy(buffer->data[channel_index] + (block_index * block_size), this->output->out[channel_index],
                   block_frames * sizeof(sample));
        }
    }
}

BufferRef AudioGraph::render_to_new_buffer(int num_frames)
{
    BufferRef buf = new Buffer(this->get_num_output_channels(), num_frames);
    this->render_to_buffer(buf);
    return buf;
}

NodeRef AudioGraph::get_output() { return this->output; }

void AudioGraph::set_output(NodeRef output_device) { this->output = output_device; }

std::list<NodeRef> AudioGraph::get_outputs()
{
    AudioOut_Abstract *output = (AudioOut_Abstract *) (this->output.get());
    return output->get_inputs();
}

std::list<std::string> AudioGraph::get_output_device_names()
{
    AudioOut_SoundIO *output = (AudioOut_SoundIO *) (this->output.get());
    return output->get_output_device_names();
}

std::list<std::string> AudioGraph::get_output_backend_names()
{
    AudioOut_SoundIO *output = (AudioOut_SoundIO *) (this->output.get());
    return output->get_output_backend_names();
}

NodeRef AudioGraph::add_node(NodeRef node)
{
    if (this->config.get_cpu_usage_limit() > 0.0 && this->cpu_usage > this->config.get_cpu_usage_limit())
    {
        throw cpu_usage_above_limit_exception();
    }

    this->scheduled_nodes.insert(node);
    return node;
}

void AudioGraph::remove_node(NodeRef node)
{
    this->scheduled_nodes.erase(node);
}

void AudioGraph::add_patch(PatchRef patch)
{
    if (this->config.get_cpu_usage_limit() > 0.0 && this->cpu_usage > this->config.get_cpu_usage_limit())
    {
        throw cpu_usage_above_limit_exception();
    }

    patch->parse();
    this->patches.insert(patch);
}

void AudioGraph::play(PatchRef patch)
{
    if (this->config.get_cpu_usage_limit() > 0.0 && this->cpu_usage > this->config.get_cpu_usage_limit())
    {
        throw cpu_usage_above_limit_exception();
    }
    if (patch->get_state() == SIGNALFLOW_PATCH_STATE_STOPPED)
    {
        throw patch_finished_playback_exception();
    }

    AudioOut_Abstract *audioout = (AudioOut_Abstract *) (this->output.get());

    /*----------------------------------------------------------------------------
     * If a Patch has been instantiated from a PatchSpec, its structure has
     * already been parsed, which means it already contains an internal index
     * of its constituent nodes (needed for things like auto-free).
     *
     * However, if it has been constructed from raw nodes, its structure needs
     * to be parsed before playback to stop unwanted things from happening
     * (such as auto-free being ignored).
     *
     * Calling patch->parse() parses the patch if it has not been parsed already.
     * This is inefficient, so it's always better to instantiate a Patch from
     * a PatchSpec. (TODO: Should instantiating patches without a PatchSpec
     * be deprecated?)
     *----------------------------------------------------------------------------*/
    patch->parse();

    audioout->add_input(patch->get_output());
    this->patches.insert(patch);
}

void AudioGraph::play(NodeRef node)
{
    if (this->config.get_cpu_usage_limit() > 0.0 && this->cpu_usage > this->config.get_cpu_usage_limit())
    {
        throw cpu_usage_above_limit_exception();
    }

    AudioOut_Abstract *output = (AudioOut_Abstract *) this->output.get();
    output->add_input(node);
}

bool AudioGraph::is_playing(NodeRef node)
{
    // TODO: This currently only works if the node is wired directly to the
    //       graph's output. To support nested nodes, should recursively
    //       traverse the outputs of the node.
    AudioOut_Abstract *audioout = (AudioOut_Abstract *) (this->output.get());
    return audioout->has_input(node);
}

void AudioGraph::stop(PatchRef patch) { this->stop(patch.get()); }

void AudioGraph::stop(Patch *patch)
{
    patches_to_remove.insert(patch);
    nodes_to_remove.insert(patch->get_output());
}

void AudioGraph::stop(NodeRef node)
{
    // TODO: Should ideally do node->is_playing() here, but this won't catch cases in
    //       which the node is embedded somewhere within the graph.
    if (!node->outputs.empty())
    {
        nodes_to_remove.insert(node);
    }
    else
    {
        throw node_not_playing_exception();
    }
}

void AudioGraph::replace(NodeRef node, NodeRef other) { nodes_to_replace.insert(std::make_pair(node, other)); }

void AudioGraph::start_recording(const std::string &filename, int num_channels)
{
    SF_INFO info;
    memset(&info, 0, sizeof(SF_INFO));
    info.frames = this->get_output_buffer_size();

    if (num_channels == 0)
    {
        num_channels = this->output->get_num_input_channels();
    }
    info.channels = num_channels;
    info.samplerate = (int) this->sample_rate;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    this->recording_num_channels = num_channels;
    this->recording_fd = sf_open(filename.c_str(), SFM_WRITE, &info);

    if (!this->recording_fd)
    {
        throw std::runtime_error(std::string("Failed to write soundfile (") + std::string(sf_strerror(NULL)) + ")");
    }
}

void AudioGraph::stop_recording() { sf_close(this->recording_fd); }

void AudioGraph::show_structure()
{
    std::cout << "AudioGraph" << std::endl;

    std::string structure = this->get_structure();
    std::cout << structure;
}

std::string AudioGraph::get_structure(NodeRef &root, int depth)
{
    std::string structure;
    structure += std::string(depth * 3, ' ');
    structure += " * " + root->name + "\n";
    for (auto pair : root->inputs)
    {
        NodeRef param_node = *(pair.second);
        if (param_node)
        {
            structure += std::string((depth + 1) * 3, ' ');
            if (param_node->get_name() == "constant")
            {
                Constant *constant = (Constant *) (param_node.get());
                structure += pair.first + ": " + std::to_string(constant->value) + "\n";
            }
            else
            {
                structure += pair.first + ":" + "\n";
                structure += this->get_structure(param_node, depth + 1);
            }
        }
    }

    return structure;
}

std::string AudioGraph::get_structure() { return this->get_structure(this->output, 0); }

void AudioGraph::poll(float frequency)
{
    if (frequency > 0)
    {
        if (this->monitor)
        {
            this->monitor->set_frequency(frequency);
            if (!this->monitor->is_running())
            {
                this->monitor->start();
            }
        }
        else
        {
            this->monitor = new AudioGraphMonitor(this, frequency);
            this->monitor->start();
        }
    }
    else
    {
        std::cout << this->get_status() << std::endl;
        if (this->monitor)
        {
            this->monitor->stop();
        }
    }
}

std::string AudioGraph::get_status()
{
    int node_count = this->get_node_count();
    std::string nodes = (node_count == 1 ? "node" : "nodes");
    int patch_count = this->get_patch_count();
    std::string patches = (patch_count == 1 ? "patch" : "patches");
    float cpu_usage = this->get_cpu_usage() * 100.0;

    /*--------------------------------------------------------------------------------
     * Would be nice to use std::format but this is only available in c++20.
     *--------------------------------------------------------------------------------*/
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << cpu_usage;
    std::string cpu_usage_str = ss.str();

    ss.clear();
    ss.str("");
    float memory_usage_mb = (this->memory_usage / 1024) / (float) 1024;
    ss << std::fixed << std::setprecision(1) << memory_usage_mb;
    std::string memory_usage_str = ss.str();

    float peak_output = signalflow_amplitude_to_db(this->peak_level);

    std::string peak_output_str;
    if (peak_output > -180)
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1) << peak_output;
        peak_output_str = out.str();
    }
    else
    {
        peak_output_str = "-";
    }

    std::string status = "AudioGraph: " + std::to_string(node_count) + " active " + nodes + ", "
        + std::to_string(patch_count) + " " + patches + ", " + cpu_usage_str + "% CPU usage, " + memory_usage_str
        + "MB memory usage, output = " + peak_output_str + "dB";

    return status;
}

int AudioGraph::get_sample_rate() { return this->sample_rate; }

void AudioGraph::set_sample_rate(int sample_rate)
{
    if (sample_rate <= 0)
    {
        throw std::runtime_error("Sample rate cannot be <= 0");
    }
    this->sample_rate = sample_rate;
}

int AudioGraph::get_output_buffer_size()
{
    if (this->output)
    {
        AudioOut_Abstract *output = (AudioOut_Abstract *) this->output.get();
        return output->get_buffer_size();
    }
    else
    {
        return 0;
    }
}

int AudioGraph::get_num_output_channels()
{
    if (this->output)
    {
        AudioOut_Abstract *output = (AudioOut_Abstract *) this->output.get();
        return output->get_num_input_channels();
    }
    else
    {
        return 0;
    }
}

int AudioGraph::get_node_count() { return this->node_count; }

int AudioGraph::get_patch_count() { return (int) this->patches.size(); }

float AudioGraph::get_cpu_usage() { return this->cpu_usage; }

size_t AudioGraph::get_memory_usage() { return this->memory_usage; }

AudioGraphConfig &AudioGraph::get_config() { return this->config; }

void AudioGraph::register_memory_alloc(size_t num_bytes) { memory_usage += num_bytes; }

void AudioGraph::register_memory_dealloc(size_t num_bytes) { memory_usage -= num_bytes; }

}
