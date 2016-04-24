#pragma once

#include "../../node.h"
#include "../../buffer.h"
#include "../../graph.h"

#include <vector>


namespace libsignal
{
	class AudioIn_Abstract : public Node
	{
	public:
        AudioIn_Abstract();
        
        virtual int init() = 0;
        virtual int start() = 0;
        virtual int close() = 0;
        
        virtual void process(sample **out, int num_samples) = 0;
	};
}
