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
        this->min_output_channels = this->max_output_channels = this->num_output_channels;
        this->num_input_channels = 0;
        this->min_input_channels = this->max_input_channels = this->num_input_channels;
    }
        
}
