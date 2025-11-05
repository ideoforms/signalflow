#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * YIN pitch tracker
 * 
 * Implements the YIN fundamental frequency estimator algorithm
 * (de Cheveigné & Kawahara, 2002)
 * 
 * Tracks pitch on channel 0 (monophonic) and outputs estimated f0 on all channels.
 *---------------------------------------------------------------------------------*/
class YinPitchTracker : public UnaryOpNode
{
public:
    YinPitchTracker(NodeRef input = 0.0,
                    NodeRef threshold = 0.1,
                    NodeRef f0_min = 50.0,
                    NodeRef f0_max = 2000.0,
                    unsigned int window_size = 1024);
    ~YinPitchTracker() override;

    virtual void process(Buffer &out, int num_frames) override;

private:
    // CMNDF threshold (typically 0..1)
    NodeRef threshold;
    NodeRef f0_min;
    NodeRef f0_max;
    unsigned int window_size;

    // Internal state
    SampleRingBuffer *input_buffer;
    std::vector<float> difference_function;
    std::vector<float> cmndf;

    float current_f0;

    // Helper methods
    void compute_difference_function(int tau_min, int tau_max);
    void compute_cmndf(int tau_min, int tau_max);
    int find_absolute_minimum(int tau_min, int tau_max, float threshold_val);
    float parabolic_interpolation(int tau);
};

REGISTER(YinPitchTracker, "yin-pitch-tracker")
}
