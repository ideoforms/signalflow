#include "signalflow/node/operators/add.h"

namespace signalflow
{

Add::Add(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "add";
}

void Add::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {

#if defined(__APPLE__)

        vDSP_vadd(input0->out[channel], 1, input1->out[channel], 1, out[channel], 1, num_frames);

#elif defined(__aarch64__)

        for (int frame = 0; frame <= num_frames - 4; frame += 4)
        {
            // Load 4 floats from each input
            float32x4_t a = vld1q_f32(&input0->out[channel][frame]);
            float32x4_t b = vld1q_f32(&input1->out[channel][frame]);

            // Vector add
            float32x4_t c = vaddq_f32(a, b);

            // Store result
            vst1q_f32(&out[channel][frame], c);
        }

#else

        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = input0->out[channel][frame] + input1->out[channel][frame];
        }

#endif
    }
}

}
