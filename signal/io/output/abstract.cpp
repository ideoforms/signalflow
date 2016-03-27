#include "abstract.h"

namespace libsignal
{
    Graph *shared_graph = NULL;
    
    AudioOut_Abstract::AudioOut_Abstract(Graph *graph)
    {
        this->graph = graph;
        shared_graph = graph;
        
        this->name = "audioout";
        // this->num_input_channels = 2;
        this->num_output_channels = 2;
        this->no_input_automix = true;
    }
    
    
    void AudioOut_Abstract::next(sample **out, int num_frames)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
            memset(out[channel], 0, num_frames * sizeof(sample));
        
        for (NodeRef input : this->inputs)
        {
            for (int channel = 0; channel < input->num_output_channels; channel++)
            {
                #ifdef __APPLE__
                
                vDSP_vadd(input->out[channel], 1, out[channel], 1, out[channel], 1, num_frames);
                
                #else
                
                for (int frame = 0; frame < num_frames; frame++)
                {
                    out[channel][frame] += input->out[channel][frame];
                }
                
                #endif
            }
        }
    }
    
} // namespace libsignal
