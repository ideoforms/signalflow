#include "signalflow/core/core.h"
#include "signalflow/core/graph-monitor.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"
#include "signalflow/node/oscillators/constant.h"

#include "signalflow/patch/patch.h"

#include "signalflow/node/io/output/abstract.h"
#include "signalflow/node/io/output/ios.h"
#include "signalflow/node/io/output/soundio.h"

#include <limits.h>
#include <string.h>

namespace signalflow
{

AudioGraph *shared_graph = nullptr;

AudioGraph::AudioGraph(AudioGraphConfig *config,
                       NodeRef output_device,
                       bool start)
{
    signalflow_init();

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
        this->output = new AudioOut(this->config.get_output_device_name(),
                                    this->config.get_sample_rate(),
                                    this->config.get_output_buffer_size());
        if (!this->output)
        {
            throw std::runtime_error("AudioGraph: Couldn't find audio output device");
        }
    }

    AudioOut_Abstract *audio_out = (AudioOut_Abstract *) this->output.get();

    if (audio_out->get_sample_rate() == 0)
    {
        throw std::runtime_error("AudioGraph: Audio output device has zero sample rate");
    }

    this->sample_rate = audio_out->get_sample_rate();
    this->node_count = 0;
    this->_node_count_tmp = 0;
    this->cpu_usage = 0.0;
    this->monitor = nullptr;

    this->recording_fd = NULL;
    this->recording_num_channels = 0;
    this->recording_buffer = (float *) calloc(SIGNALFLOW_DEFAULT_BLOCK_SIZE * SIGNALFLOW_MAX_CHANNELS, sizeof(float));

    if (start)
    {
        this->start();
    }
}

void AudioGraph::start()
{
    AudioOut_Abstract *audio_out = (AudioOut_Abstract *) this->output.get();
    audio_out->start();
}

void AudioGraph::stop()
{
    AudioOut_Abstract *audioout = (AudioOut_Abstract *) this->output.get();
    audioout->stop();
}

void AudioGraph::clear()
{
    AudioOut_Abstract *audioout = (AudioOut_Abstract *) this->output.get();
    auto inputs = audioout->get_inputs();
    for (auto input : inputs)
    {
        audioout->remove_input(input);
    }
    this->node_count = 0;
}

AudioGraph::~AudioGraph()
{
    AudioOut_Abstract *audioout = (AudioOut_Abstract *) this->output.get();
    audioout->destroy();
    shared_graph = nullptr;
}

void AudioGraph::wait(float time)
{
    if (time == -1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(std::numeric_limits<long>::max()));
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1000.0f * time)));
    }
}

void AudioGraph::render_subgraph(const NodeRef &node, int num_frames)
{
    /*------------------------------------------------------------------------
     * If this node has already been processed this timestep, return.
     *-----------------------------------------------------------------------*/
    if (node->has_rendered)
    {
        return;
    }

    if (!(node->inputs.size() > 0 || node->name == "constant" || node->name == "audioout" || node->name == "audioin"))
    {
        signalflow_debug("Node %s has no registered inputs", node->name.c_str());
    }

    /*------------------------------------------------------------------------
     * Pull our inputs before we generate our own outputs.
     *-----------------------------------------------------------------------*/
    for (auto input : node->inputs)
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
             * itself (for example, Multiply(new SineOscillator(440), new LinearPanner(2, ...)))
             *
             * A few nodes must prevent automatic input up-mixing from happening.
             * These include ChannelArray and AudioOut.
             *
             * Some partially-initialised nodes (e.g. BufferPlayer with a not-yet-
             * populated Buffer) will have num_output_channels == 0. Don't try to
             * upmix a void output.
             *-----------------------------------------------------------------------*/
            if (input_node->get_num_output_channels() < node->get_num_input_channels() && !node->no_input_upmix && input_node->get_num_output_channels() > 0)
            {
                signalflow_debug("Upmixing %s (%s wants %d channels, %s only produces %d)", input_node->name.c_str(),
                                 node->name.c_str(), node->get_num_input_channels(), input_node->name.c_str(), input_node->get_num_output_channels());

                /*------------------------------------------------------------------------
                 * Ensure the input node's output buffer re-allocation has been done.
                 * Reallocation for inputs is automatically done in Node::update_channels.
                 *-----------------------------------------------------------------------*/
                if (input_node->get_num_output_channels_allocated() < node->get_num_input_channels())
                {
                    throw std::runtime_error("Input node does not have enough buffers allocated (need " + std::to_string(node->get_num_input_channels()) + ", got " + std::to_string(input_node->get_num_output_channels_allocated()));
                }

                /*------------------------------------------------------------------------
                 * If we generate 2 channels but have 6 channels demanded, repeat
                 * them: [ 0, 1, 0, 1, 0, 1 ]
                 *-----------------------------------------------------------------------*/
                for (int out_channel_index = input_node->get_num_output_channels();
                     out_channel_index < node->get_num_input_channels();
                     out_channel_index++)
                {
                    int in_channel_index = out_channel_index % input_node->get_num_output_channels();
                    memcpy(input_node->out[out_channel_index],
                           input_node->out[in_channel_index],
                           num_frames * sizeof(sample));
                }
            }
        }
    }

    node->_process(node->out, num_frames);

    if (node->name != "constant")
    {
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

    /*------------------------------------------------------------------------
     * Disconnect any nodes and patches that are scheduled to be removed.
     *-----------------------------------------------------------------------*/
    for (auto node : nodes_to_remove)
    {
        AudioOut_Abstract *output = (AudioOut_Abstract *) this->output.get();
        output->remove_input(node);
    }
    nodes_to_remove.clear();

    for (auto patch : patches_to_remove)
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
    this->patches_to_remove.clear();

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
    for (auto input : node->inputs)
    {
        NodeRef input_node = *(input.second);
        if (input_node && input_node->has_rendered)
        {
            this->reset_subgraph(input_node);
        }
    }
}

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
                this->recording_buffer[frame_index * this->recording_num_channels + channel_index] = this->output->out[channel_index][frame_index];
            }
        }
        sf_writef_float(this->recording_fd, this->recording_buffer, num_frames);
    }

    /*------------------------------------------------------------------------
     * Calculate CPU usage (approximately) by measuring the % of time
     * within the audio I/O callback that was used for processing.
     *-----------------------------------------------------------------------*/
    double t1 = signalflow_timestamp();
    double dt = t1 - t0;
    double t_max = (double) num_frames / this->sample_rate;
    this->cpu_usage = dt / t_max;
    if (this->cpu_usage > 1.0)
    {
        std::cerr << "Warning: buffer overrun?" << std::endl;
    }
}

void AudioGraph::render_to_buffer(BufferRef buffer, int block_size)
{
    // TODO get_num_output_channels()
    int channel_count = buffer->get_num_channels();
    if (channel_count > this->output->num_input_channels)
    {
        throw std::runtime_error("Buffer cannot have more channels than the audio graph (" + std::to_string(channel_count) + " != " + std::to_string(this->output->num_input_channels) + ")");
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
            memcpy(buffer->data[channel_index] + (block_index * block_size),
                   this->output->out[channel_index],
                   block_frames * sizeof(sample));
        }
    }
}

NodeRef AudioGraph::get_output()
{
    return this->output;
}

std::list<NodeRef> AudioGraph::get_outputs()
{
    AudioOut_Abstract *output = (AudioOut_Abstract *) (this->output.get());
    return output->get_inputs();
}

NodeRef AudioGraph::add_node(NodeRef node)
{
    this->scheduled_nodes.insert(node);
    return node;
}

void AudioGraph::remove_node(NodeRef node)
{
    this->scheduled_nodes.erase(node);
}

void AudioGraph::play(PatchRef patch)
{
    AudioOut_Abstract *output = (AudioOut_Abstract *) (this->output.get());

    /*----------------------------------------------------------------------------
     * If a Patch has been instantiated from a PatchSpec, its structure has
     * already been parsed, which means it already contains an internal index
     * of its constituent nodes (needed for things like auto-free).
     *
     * However, if it has been constructed from raw nodes, its structure needs
     * to be parsed before playback to stop unwanted things from happening.
     * Calling patch->parse() parses the patch if it has not been parsed already.
     *----------------------------------------------------------------------------*/
    patch->parse();

    output->add_input(patch->output);
    this->patches.insert(patch);
}

void AudioGraph::play(NodeRef node)
{
    AudioOut_Abstract *output = (AudioOut_Abstract *) this->output.get();
    output->add_input(node);
}

void AudioGraph::stop(PatchRef patch)
{
    this->stop(patch.get());
}

void AudioGraph::stop(Patch *patch)
{
    patches_to_remove.insert(patch);
    nodes_to_remove.insert(patch->output);
}

void AudioGraph::stop(NodeRef node)
{
    nodes_to_remove.insert(node);
}

void AudioGraph::replace(NodeRef node, NodeRef other)
{
    nodes_to_replace.insert(std::make_pair(node, other));
}

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

void AudioGraph::stop_recording()
{
    sf_close(this->recording_fd);
}

void AudioGraph::show_structure()
{
    std::cout << "AudioGraph" << std::endl;
    this->show_structure(this->output, 0);
}

void AudioGraph::show_structure(NodeRef &root, int depth)
{
    std::cout << std::string(depth * 2, ' ');
    std::cout << " * " << root->name << std::endl;
    for (auto pair : root->inputs)
    {
        std::cout << std::string((depth + 1) * 2 + 1, ' ');

        NodeRef param_node = *(pair.second);
        if (param_node->name == "constant")
        {
            Constant *constant = (Constant *) (param_node.get());
            std::cout << pair.first << ": " << constant->value << std::endl;
        }
        else
        {
            std::cout << pair.first << ":" << std::endl;
            this->show_structure(param_node, depth + 1);
        }
    }
}

void AudioGraph::show_status(float frequency)
{
    if (frequency > 0)
    {
        if (this->monitor)
        {
            throw std::runtime_error("AudioGraph is already polling state");
        }
        this->monitor = new AudioGraphMonitor(this, frequency);
        this->monitor->start();
    }
}

std::string AudioGraph::get_status()
{
    int node_count = this->get_node_count();
    std::string nodes = (node_count == 1 ? "node" : "nodes");
    int patch_count = this->get_patch_count();
    std::string patches = (patch_count == 1 ? "patch" : "patches");
    float cpu_usage = this->get_cpu_usage() * 100.0;
    std::string status = "AudioGraph: " + std::to_string(node_count) + " active " + nodes + ", " + std::to_string(patch_count) + " " + patches + ", " + std::to_string(cpu_usage) + "% CPU usage";

    return status;
}

int AudioGraph::get_sample_rate()
{
    return this->sample_rate;
}

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

int AudioGraph::get_node_count()
{
    return this->node_count;
}

int AudioGraph::get_patch_count()
{
    return (int) this->patches.size();
}

float AudioGraph::get_cpu_usage()
{
    return this->cpu_usage;
}

AudioGraphConfig &AudioGraph::get_config()
{
    return this->config;
}

}
