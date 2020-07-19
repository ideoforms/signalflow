#include "signalflow/core/core.h"
#include "signalflow/core/graph-monitor.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"
#include "signalflow/node/oscillators/constant.h"

#include "signalflow/patch/patch.h"

#include "signalflow/node/io/output/abstract.h"
#include "signalflow/node/io/output/ios.h"
#include "signalflow/node/io/output/soundio.h"

#include <string.h>
#include <sys/time.h>
#include <unistd.h>

namespace signalflow
{

AudioGraph *shared_graph = NULL;

AudioGraph::AudioGraph()
{
    signal_init();

    if (shared_graph)
    {
        throw graph_already_created_exception();
    }
    shared_graph = this;

    AudioOut *audioout = new AudioOut(this);
    this->output = audioout;
    this->sample_rate = audioout->sample_rate;
    this->node_count = 0;
    this->_node_count_tmp = 0;
    this->cpu_usage = 0.0;
    this->monitor = NULL;
}

void AudioGraph::start()
{
    AudioOut *audioout = (AudioOut *) this->output.get();
    audioout->start();
}

AudioGraph::~AudioGraph()
{
    AudioOut *audioout = (AudioOut *) this->output.get();
    audioout->destroy();
    shared_graph = NULL;
}

void AudioGraph::wait(float time)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double t0 = tv.tv_sec + tv.tv_usec / 1000000.0;

    while (true)
    {
        usleep(10000);
        if (time)
        {
            gettimeofday(&tv, NULL);
            double t1 = tv.tv_sec + tv.tv_usec / 1000000.0;
            double dt = t1 - t0;
            if (dt > time)
            {
                break;
            }
        }
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
        signal_debug("Node %s has no registered inputs", node->name.c_str());
    }

    /*------------------------------------------------------------------------
     * Pull our inputs before we generate our own outputs.
     *-----------------------------------------------------------------------*/
    for (auto param : node->inputs)
    {
        NodeRef param_node = *(param.second);
        if (param_node)
        {
            this->render_subgraph(param_node, num_frames);

            /*------------------------------------------------------------------------
             * Automatic input upmix.
             *
             * If the input node produces less channels than demanded, automatically
             * up-mix its output by replicating the existing channels. This allows
             * operations between multi-channel and mono-channel inputs to work
             * seamlessly without any additional implementation within the node
             * itself (for example, Multiply(new Sine(440), new LinearPanner(2, ...)))
             *
             * A few nodes must prevent automatic input up-mixing from happening.
             * These include ChannelArray and AudioOut.
             *
             * Some partially-initialised nodes (e.g. BufferPlayer with a not-yet-
             * populated Buffer) will have num_output_channels == 0. Don't try to
             * upmix a void output.
             *-----------------------------------------------------------------------*/
            if (param_node->num_output_channels < node->num_input_channels && !node->no_input_upmix && param_node->num_output_channels > 0)
            {
                signal_debug("Upmixing %s (%s wants %d channels, %s only produces %d)", param_node->name.c_str(),
                             node->name.c_str(), node->num_input_channels, param_node->name.c_str(), param_node->num_output_channels);

                /*------------------------------------------------------------------------
                 * If we generate 2 channels but have 6 channels demanded, repeat
                 * them: [ 0, 1, 0, 1, 0, 1 ]
                 *-----------------------------------------------------------------------*/
                for (int out_channel_index = param_node->num_output_channels;
                     out_channel_index < node->num_input_channels;
                     out_channel_index++)
                {
                    int in_channel_index = out_channel_index % param_node->num_output_channels;
                    memcpy(param_node->out[out_channel_index],
                           param_node->out[in_channel_index],
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
    AudioOut_Abstract *audio_output = (AudioOut_Abstract *) this->output.get();

    /*------------------------------------------------------------------------
     * Disconnect any nodes and patches that are scheduled to be removed.
     *-----------------------------------------------------------------------*/
    for (auto node : nodes_to_remove)
    {
        audio_output->remove_input(node);
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
    double t0 = signal_timestamp();

    this->reset_graph();
    this->render_subgraph(this->output, num_frames);
    this->node_count = this->_node_count_tmp;
    signal_debug("AudioGraph: pull %d frames, %d nodes", num_frames, this->node_count);

    /*------------------------------------------------------------------------
     * Calculate CPU usage (approximately) by measuring the % of time
     * within the audio I/O callback that was used for processing.
     *-----------------------------------------------------------------------*/
    double t1 = signal_timestamp();
    double dt = t1 - t0;
    double t_max = (double) num_frames / this->sample_rate;
    this->cpu_usage = dt / t_max;
}

void AudioGraph::render_to_buffer(BufferRef buffer, int block_size)
{
    // TODO get_num_output_channels()
    int channel_count = buffer->get_num_channels();
    if (buffer->get_num_channels() > this->output->num_output_channels)
    {
        throw std::runtime_error("Buffer cannot have more channels than the audio graph (" + std::to_string(buffer->get_num_channels()) + " != " + std::to_string(channel_count) + ")");
    }
    int block_count = ceilf((float) buffer->get_num_frames() / block_size);

    for (int block_index = 0; block_index < block_count; block_index++)
    {
        int block_frames = (block_index == block_count - 1 ? buffer->get_num_frames() % block_size : block_size);
        this->render(block_frames);
        for (int channel_index = 0; channel_index < channel_count; channel_index++)
        {
            memcpy(buffer->data[channel_index] + (block_index * block_size),
                   this->output->out[channel_index],
                   block_frames * sizeof(sample));
        }
    }
}

//void AudioGraph::process(const NodeRef &root, int num_frames, int block_size)
//{
//    /*------------------------------------------------------------------------
//     * Updating the routing from the tip.
//     * This normally happens when the root is connected to the graph's
//     * audio out. Can this be improved?
//     *-----------------------------------------------------------------------*/
//    root->update_channels();
//
//    int index = 0;
//    signal_debug("AudioGraph: Performing offline process of %d frames", num_frames);
//    while (index < (num_frames - block_size))
//    {
//        signal_debug("AudioGraph: Processing frame %d...", index);
//        this->reset_graph();
//        this->render_subgraph(root, block_size);
//        index += block_size;
//    }
//
//    /*------------------------------------------------------------------------
//     * Process remaining samples.
//     *-----------------------------------------------------------------------*/
//    if (index < num_frames)
//    {
//        signal_debug("AudioGraph: Processing remaining %d samples", num_frames - index);
//        this->reset_graph();
//        this->render_subgraph(root, num_frames - index);
//    }
//
//    signal_debug("AudioGraph: Offline process completed");
//}

NodeRef AudioGraph::get_output()
{
    return this->output;
}

void AudioGraph::play(PatchRef patch)
{
    AudioOut_Abstract *output = (AudioOut_Abstract *) (this->output.get());
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

}
