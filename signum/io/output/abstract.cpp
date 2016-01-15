#include "abstract.h"

namespace signum
{
    Graph *shared_graph = NULL;
    
    AudioOut_Abstract::AudioOut_Abstract(Graph *graph)
    {
        this->graph = graph;
        shared_graph = graph;
        
        this->name = "audioout";
        // this->channels_in = 2;
        this->channels_out = 2;
    }
    
    
    void AudioOut_Abstract::next(sample **out, int num_frames)
    {
        for (int channel = 0; channel < this->channels_out; channel++)
            memset(out[channel], 0, num_frames * sizeof(sample));
        
        for (int index = 0; index < this->inputs.size(); index++)
        {
            UnitRef unit = this->inputs[index];
            for (int channel = 0; channel < unit->channels_out; channel++)
            {
                #ifdef __APPLE__
                
                vDSP_vadd(unit->out[channel], 1, out[channel], 1, out[channel], 1, num_frames);
                
                #else
                
                for (int frame = 0; frame < num_frames; frame++)
                {
                    out[channel][frame] += unit->out[channel][frame];
                }
                
                #endif
            }
        }
    }
    
} // namespace signum
