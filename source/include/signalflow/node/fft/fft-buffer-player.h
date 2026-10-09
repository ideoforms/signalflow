#pragma once

#include "signalflow/buffer/fftbuffer.h"
#include "signalflow/node/fft/fft-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFTBufferPlayer.
 * Plays from a buffer of audio spectra in mag/phase format.
 *---------------------------------------------------------------------------------*/
class FFTBufferPlayer : public FFTNode
{
public:
    FFTBufferPlayer(FFTBufferRef buffer = nullptr, NodeRef rate = 1.0);
    ~FFTBufferPlayer();

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER,
                         float value = SIGNALFLOW_NULL_FLOAT) override;

private:
    FFTBufferRef buffer;
    NodeRef rate;

    int current_frame_index;
    int next_frame_counter;
    int flush_count;
};

REGISTER(FFTBufferPlayer, "fft-buffer-player")
}
