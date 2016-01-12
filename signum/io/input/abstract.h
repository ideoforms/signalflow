#pragma once

#include "../../unit.h"
#include "../../buffer.h"
#include "../../graph.h"

#include <vector>


namespace signum
{
	class AudioIn_Abstract : public Unit
	{
	public:
        AudioIn_Abstract();
        
        virtual int init() = 0;
        virtual int start() = 0;
        virtual int close() = 0;
        
        virtual void next(sample **out, int num_samples) = 0;
	};
}
