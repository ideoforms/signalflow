#include "signalflow/node/fft/fft-buffer-player.h"

namespace signalflow
{

FFTBufferPlayer::FFTBufferPlayer(FFTBufferRef buffer, NodeRef rate)
    : FFTNode(buffer->get_fft_size(), buffer->get_hop_size(), 0, false), buffer(buffer), rate(rate)
{
    this->name = "fft-buffer-player";
    this->current_frame_index = 0;
    this->next_frame_counter = 0;
    this->flush_count = 0;
    this->create_input("rate", this->rate);
}

FFTBufferPlayer::~FFTBufferPlayer()
{
}

void FFTBufferPlayer::process(Buffer &out, int num_frames)
{
    this->num_hops = 0;
    for (int frame = 0; frame < num_frames; frame++)
    {
        next_frame_counter--;
        float rate = this->rate->out[0][frame];
        if (next_frame_counter <= 0 || flush_count > 0)
        {
            memcpy(this->out.data[this->num_hops],
                   this->buffer->get_frame(this->current_frame_index),
                   this->buffer->get_num_bins() * 2 * sizeof(float));
            this->next_frame_counter = (int) (this->hop_size * rate);
            this->num_hops += 1;
            this->current_frame_index += 1;
            if (flush_count > 0)
                flush_count--;
        }
    }
}

void FFTBufferPlayer::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_TRIGGER_SET_POSITION)
    {
        float position_in_samples = (value * this->buffer->get_sample_rate());
        float position_in_fft_frames = position_in_samples / hop_size;
        int position_in_fft_frames_int = (int) roundf(position_in_fft_frames);
        this->current_frame_index = position_in_fft_frames_int;
        this->next_frame_counter = 0;
    }
    else if (name == "flush")
    {
        this->flush_count = 4;
    }
    else
    {
        throw std::runtime_error("Unknown trigger: " + name);
    }
}

}
