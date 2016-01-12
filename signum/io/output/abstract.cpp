#include "abstract.h"

namespace signum
{
    Graph *shared_graph = NULL;
    
    AudioOut_Abstract::AudioOut_Abstract(Graph *graph)
    {
        this->graph = graph;
        shared_graph = graph;
        
        this->name = "audioout";
        this->channels_out = 2;
    }
    
} // namespace signum