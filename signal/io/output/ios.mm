#include "ios.h"

#if TARGET_OS_IPHONE

#include "../../graph.h"

#ifdef __OBJC__
#include "AudioIOManager.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>


namespace libsignal
{
    
extern AudioGraph *shared_graph;
    
void audio_callback(float **data, int num_channels, int num_frames)
{
    shared_graph->pull_input(num_frames);
    
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < num_channels; channel++)
        {
            data[channel][frame] = shared_graph->output->out[channel][frame];
        }
    }
}
    
AudioOut_iOS::AudioOut_iOS(AudioGraph *graph) : AudioOut_Abstract(graph)
{
    this->init();
}

int AudioOut_iOS::init()
{
    AudioIOManager *ioManager = [[AudioIOManager alloc] initWithCallback:audio_callback];
    [ioManager start];
    
	return 0;
}

int AudioOut_iOS::start()
{
	return 0;
}

int AudioOut_iOS::close()
{
	return 0;
}


} // namespace libsignal

#endif /* TARGET_OS_IPHONE */

