#pragma once

#include "signalflow/buffer/buffer.h"

namespace signalflow
{

class Grain
{
public:
    Grain(BufferRef buffer, int phase, int length, float rate, float pan, bool wrap);

    /**--------------------------------------------------------------------------------*
     * Returns true if the grain has completed its requested duration,
     * or, if loop is false, reached the edge of the input buffer.
     *---------------------------------------------------------------------------------*/
    bool is_finished();

    /**--------------------------------------------------------------------------------*
     * Step forward one sample.
     *---------------------------------------------------------------------------------*/
    void step();

    /**--------------------------------------------------------------------------------*
     * Returns a float [0..1] indicating the progress through the grain.
     *---------------------------------------------------------------------------------*/
    double get_progress();

    /**--------------------------------------------------------------------------------*
     * Leave properties public so that Node classes can reference them directly.
     *---------------------------------------------------------------------------------*/
    BufferRef buffer;
    double phase;
    int length_samples;
    double samples_processed;
    float rate;
    float pan;
    bool wrap;
};

}
