#pragma once

#include "signalflow/buffer/fftbuffer.h"
#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFTBufferPlayer.
 * Plays from a buffer of audio spectra in mag/phase format.
 *---------------------------------------------------------------------------------*/
class FFTBufferPlayer : public FFTNode
{
public:
    FFTBufferPlayer(FFTBufferRef buffer = nullptr);
    ~FFTBufferPlayer();

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER,
                         float value = SIGNALFLOW_NULL_FLOAT) override;

private:
    FFTBufferRef buffer;
    int current_frame_index;
    int next_frame_counter;
    int flush_count;
};

REGISTER(FFTBufferPlayer, "fft-buffer-player")
}
