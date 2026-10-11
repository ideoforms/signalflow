#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

#define SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE 256

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * 2D Perlin noise, sampled at the coordinates given by x and y.
 * Successive octaves are summed with doubling frequency, each scaled in amplitude
 * by detail relative to the previous octave. Output is in the range [-1, 1].
 * The noise field tiles every 256 units.
 *---------------------------------------------------------------------------------*/
class PerlinNoise : public StochasticNode
{
public:
    PerlinNoise(NodeRef x = 0.0,
                NodeRef y = 0.0,
                int num_octaves = 4,
                NodeRef detail = 0.5,
                NodeRef reset = nullptr);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void set_seed(unsigned long int seed) override;

private:
    void generate_field();
    float get_noise(float x, float y);

    NodeRef x;
    NodeRef y;
    int num_octaves;
    NodeRef detail;

    float gradients_x[SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE];
    float gradients_y[SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE];
    int permutation[SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE * 2];
};

REGISTER(PerlinNoise, "perlin-noise")
}
