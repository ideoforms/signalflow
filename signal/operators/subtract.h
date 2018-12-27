#include "../constants.h"
#include "../node.h"

namespace libsignal
{

    class Subtract : public BinaryOpNode
    {

    public:

        Subtract(NodeRef a = 0, NodeRef b = 0) : BinaryOpNode(a, b)
        {
            this->name = "subtract";
        }

        virtual void process(sample **out, int num_frames)
        {
            for (int frame = 0; frame < num_frames; frame++)
            {
                for (int channel = 0; channel < this->num_output_channels; channel++)
                {
                    out[channel][frame] = input0->out[channel][frame] - input1->out[channel][frame];
                }
            }
        }
    };

    REGISTER(Subtract, "subtract");

}
