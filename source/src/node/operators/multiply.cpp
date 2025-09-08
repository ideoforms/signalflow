#include "signalflow/node/operators/multiply.h"

#if defined(__aarch64__)
#include <arm_neon.h>
#endif

namespace signalflow
{

Multiply::Multiply(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "multiply";
}

void Multiply::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
#if defined(__APPLE__)
        vDSP_vmul(input0->out[channel], 1, input1->out[channel], 1, out[channel], 1, num_frames);
#elif defined(__aarch64__)
        for (int frame = 0; frame <= num_frames - 4; frame += 4)
        {
            // Load 4 floats from each input
            float32x4_t a = vld1q_f32(&input0->out[channel][frame]);
            float32x4_t b = vld1q_f32(&input1->out[channel][frame]);

            // Vector multiply
            float32x4_t c = vmulq_f32(a, b);

            // Store result
            vst1q_f32(&out[channel][frame], c);
        }
#else
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = input0->out[channel][frame] * input1->out[channel][frame];
#endif
    }
}

}

