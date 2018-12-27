#pragma once

#include "../node.h"

namespace libsignal
{
    class Width : public UnaryOpNode
    {
        public:
            Width(NodeRef input = 0, NodeRef width = 1) : UnaryOpNode(input), width(width)
            {
                this->name = "width";
                this->add_input("width", this->width);

                this->num_input_channels = 2;
                this->num_output_channels = 2;
            }

            NodeRef width;

            virtual void process(sample **out, int num_frames);
    };

    REGISTER(Width, "width");
}
