#ifdef __APPLE__

#include "signalflow/node/processors/panning/binaural-panner.h"
#include "signalflow/core/graph.h"

namespace signalflow
{

const int N = 512;

BinauralPanner::BinauralPanner(NodeRef input,
                               NodeRef azimuth,
                               NodeRef elevation,
                               std::string sofa_path)
    : input(input), azimuth(azimuth), elevation(elevation)
{
    this->name = "binaural-panner";
    this->set_channels(1, 2);

    this->create_input("input", this->input);
    this->create_input("azimuth", this->azimuth);
    this->create_input("elevation", this->elevation);

    int err = 0;
    int filter_length;
    hrtf = mysofa_open(sofa_path.c_str(), this->get_graph()->get_sample_rate(), &filter_length, &err);
    // hrtf = mysofa_open("MIT_KEMAR_normal_pinna.sofa", this->get_graph()->get_sample_rate(), &filter_length, &err);

    if (!hrtf)
    {
        return;
    }

    // Zero-pad to 2048, to work around signalflow bug with convolution when filter length < block size
    // (would normally use filter_length)
    memset(ir_l, 0, sizeof(float) * N);
    memset(ir_r, 0, sizeof(float) * N);

    fft_l0 = new FFT(input, N, N / 2, N / 2, false);
    fft_r0 = new FFT(input, N, N / 2, N / 2, false);
    fft_l1 = new FFT(input, N, N / 2, N / 2, false);
    fft_r1 = new FFT(input, N, N / 2, N / 2, false);
    ir_buf_l = new Buffer(1, N);
    ir_buf_r = new Buffer(1, N);
    convolve_l0 = new FFTConvolve(fft_l0, ir_buf_l);
    convolve_r0 = new FFTConvolve(fft_r0, ir_buf_r);
    convolve_l1 = new FFTConvolve(fft_l1, ir_buf_l);
    convolve_r1 = new FFTConvolve(fft_r1, ir_buf_r);
    ifft_l0 = new IFFT(convolve_l0);
    ifft_r0 = new IFFT(convolve_r0);
    ifft_l1 = new IFFT(convolve_l1);
    ifft_r1 = new IFFT(convolve_r1);
}


void BinauralPanner::process(Buffer &out, int num_frames)
{
    // Convert from -1..+1 to mysofa's coordinate system, which is in degrees anticlockwise from the X-axis
    sample azimuth = -1 * (this->azimuth->out[0][0] / M_PI) * 180;
    sample elevation = this->elevation->out[0][0] / M_PI * 180;

    while (azimuth > 180)
        azimuth -= 360;
    while (azimuth < -180)
        azimuth += 360;

    convolve_l0->set_buffer("buffer", ir_buf_l);
    convolve_r0->set_buffer("buffer", ir_buf_r);

    float coords[] = {azimuth, elevation, 1.0};
    mysofa_s2c(coords);
    mysofa_getfilter_float(hrtf, coords[0], coords[1], coords[2], ir_l, ir_r, &delay_l, &delay_r);
    memcpy(ir_buf_l->data[0], ir_l, sizeof(float) * N);
    memcpy(ir_buf_r->data[0], ir_r, sizeof(float) * N);

    convolve_l1->set_buffer("buffer", ir_buf_l);
    convolve_r1->set_buffer("buffer", ir_buf_r);

    fft_l0->process(num_frames);
    fft_r0->process(num_frames);
    convolve_l0->process(num_frames);
    convolve_r0->process(num_frames);
    ifft_l0->process(num_frames);
    ifft_r0->process(num_frames);

    fft_l1->process(num_frames);
    fft_r1->process(num_frames);
    convolve_l1->process(num_frames);
    convolve_r1->process(num_frames);
    ifft_l1->process(num_frames);
    ifft_r1->process(num_frames);

    // memcpy(out[0], ifft_l0->out[0], num_frames * sizeof(sample));
    // memcpy(out[1], ifft_r0->out[0], num_frames * sizeof(sample));
    for (int frame = 0; frame < num_frames; frame++)
    {
        float frame_frac = (float) frame / (num_frames - 1);
        out[0][frame] = ifft_l0->out[0][frame] * (1 - frame_frac) + ifft_l1->out[0][frame] * frame_frac;
        out[1][frame] = ifft_r0->out[0][frame] * (1 - frame_frac) + ifft_r1->out[0][frame] * frame_frac;
    }

    // Ensure continuity between blocks by copying the tail of the previous block's output to the front of the next block's output
    memcpy(ifft_l0->out[0], ifft_l1->out[0],
           ifft_l1->get_output_buffer_length() * sizeof(sample));
    memcpy(ifft_r0->out[0], ifft_r1->out[0],
           ifft_r1->get_output_buffer_length() * sizeof(sample));
}

}

#endif
