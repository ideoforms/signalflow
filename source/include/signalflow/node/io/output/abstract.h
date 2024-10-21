#pragma once

#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

class AudioOut_Abstract : public Node
{
public:
    AudioOut_Abstract();
    virtual void process(Buffer &out, int num_samples);

    virtual void init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void destroy() = 0;

    virtual void add_input(NodeRef node);
    virtual void remove_input(NodeRef node);
    virtual void replace_input(NodeRef node, NodeRef other);
    virtual bool has_input(NodeRef node);
    std::list<NodeRef> get_inputs();

    virtual void set_channels(int num_input_channels, int num_output_channels);

    /**--------------------------------------------------------------------------------
     * Returns the audio output's sample rate. Note that this may not be the
     * same as the audio hardware's sample rate if the user has specified
     * a non-zero sample rate in AudioGraphConfig.
     *-------------------------------------------------------------------------------*/
    unsigned int get_sample_rate();

    /**--------------------------------------------------------------------------------
     * Returns the buffer size observed by the audio HAL. Note that this is
     * served by miniaudio.
     *
     * @return The buffer size, in frames.
     *-------------------------------------------------------------------------------*/
    unsigned int get_buffer_size();

protected:
    unsigned int sample_rate = 0;
    unsigned int buffer_size = 0;
    std::list<NodeRef> audio_inputs;
    int input_index;
};

} // namespace signalflow
