#include "abstract.h"

namespace libsignal
{
    AudioIn_Abstract *shared_in = NULL;
    
    AudioIn_Abstract::AudioIn_Abstract()
    {
        if (shared_in)
            throw std::runtime_error("Multiple AudioIn nodes are not yet supported.");
        
        shared_in = this;
        
        this->name = "audioin";
        this->num_output_channels = 2;
    }
        
}
