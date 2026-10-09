#pragma once

#include "signalflow/node/node.h"

#include "signalflow/node/fft/fft.h"
#include "signalflow/node/fft/ifft.h"
#include "signalflow/node/fft/processors/fft-convolve.h"

#include <mysofa.h>


namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Binaural panner.
 *---------------------------------------------------------------------------------*/
class BinauralPanner : public Node
{
public:
    BinauralPanner(NodeRef input = 0,
                   NodeRef azimuth = 0.0,
                   NodeRef elevation = 0.0,
                   std::string sofa_path = "P0099_Windowed_48kHz.sofa");

    virtual void process(Buffer &out, int num_frames) override;

    NodeRef input;
    NodeRef azimuth;
    NodeRef elevation;

private:
    struct MYSOFA_EASY *hrtf = NULL;

    NodeRef fft_l0, fft_r0;
    NodeRef fft_l1, fft_r1;
    NodeRef convolve_l0, convolve_r0;
    NodeRef convolve_l1, convolve_r1;
    NodeRef ifft_l0, ifft_r0;
    NodeRef ifft_l1, ifft_r1;
    float ir_l[2048];
    float ir_r[2048];
    BufferRef ir_buf_l;
    BufferRef ir_buf_r;
    float delay_l;
    float delay_r;


};

REGISTER(BinauralPanner, "binaural-panner")
}
