#pragma once

#if __APPLE__

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE

#define AudioOut AudioOut_iOS

#include "abstract.h"


#include "../../unit.h"
#include "../../graph.h"

namespace signum
{
    
    class AudioOut_iOS : public AudioOut_Abstract
    {
    public:
        AudioOut_iOS(Graph *graph);
        
        virtual int init() override;
        virtual int start() override;
        virtual int close() override;

        
    };

} // namespace signum

#endif

#endif