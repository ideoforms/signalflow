#include "signalflow/node/stochastic/perlin-noise.h"

#include <cmath>
#include <stdexcept>

namespace signalflow
{

/*--------------------------------------------------------------------------------
 * Quintic fade curve, giving continuous first and second derivatives at
 * lattice boundaries.
 *--------------------------------------------------------------------------------*/
static inline float signalflow_perlin_fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static inline float signalflow_perlin_lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

PerlinNoise::PerlinNoise(NodeRef x, NodeRef y, int num_octaves, NodeRef detail, NodeRef reset)
    : StochasticNode(reset), x(x), y(y), num_octaves(num_octaves), detail(detail)
{
    if (num_octaves < 1)
    {
        throw std::runtime_error("PerlinNoise: num_octaves must be at least 1");
    }

    this->name = "perlin-noise";
    this->create_input("x", this->x);
    this->create_input("y", this->y);
    this->create_input("detail", this->detail);

    /*--------------------------------------------------------------------------------
     * StochasticNode's constructor has already seeded the RNG, but its virtual
     * call to set_seed() cannot dispatch to this subclass, so generate the
     * field explicitly here.
     *--------------------------------------------------------------------------------*/
    this->generate_field();
}

void PerlinNoise::set_seed(unsigned long int seed)
{
    this->StochasticNode::set_seed(seed);
    this->generate_field();
}

void PerlinNoise::generate_field()
{
    /*--------------------------------------------------------------------------------
     * Pseudorandom unit gradient vectors, indexed by hashing lattice coordinates
     * through a shuffled permutation table (doubled to avoid wrapping indices).
     *--------------------------------------------------------------------------------*/
    for (int i = 0; i < SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE; i++)
    {
        float theta = this->random_uniform(0, M_PI * 2);
        this->gradients_x[i] = cosf(theta);
        this->gradients_y[i] = sinf(theta);
        this->permutation[i] = i;
    }

    for (int i = SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE - 1; i > 0; i--)
    {
        int j = (int) this->random_uniform(0, i + 1);
        std::swap(this->permutation[i], this->permutation[j]);
    }

    for (int i = 0; i < SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE; i++)
    {
        this->permutation[i + SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE] = this->permutation[i];
    }
}

float PerlinNoise::get_noise(float x, float y)
{
    const int mask = SIGNALFLOW_PERLIN_NOISE_TABLE_SIZE - 1;

    float x_floor = floorf(x);
    float y_floor = floorf(y);
    float fx = x - x_floor;
    float fy = y - y_floor;
    int x0 = (int) x_floor & mask;
    int y0 = (int) y_floor & mask;
    int x1 = (x0 + 1) & mask;
    int y1 = (y0 + 1) & mask;

    int g00 = this->permutation[this->permutation[x0] + y0];
    int g10 = this->permutation[this->permutation[x1] + y0];
    int g01 = this->permutation[this->permutation[x0] + y1];
    int g11 = this->permutation[this->permutation[x1] + y1];

    float v00 = this->gradients_x[g00] * fx + this->gradients_y[g00] * fy;
    float v10 = this->gradients_x[g10] * (fx - 1) + this->gradients_y[g10] * fy;
    float v01 = this->gradients_x[g01] * fx + this->gradients_y[g01] * (fy - 1);
    float v11 = this->gradients_x[g11] * (fx - 1) + this->gradients_y[g11] * (fy - 1);

    float u = signalflow_perlin_fade(fx);
    float v = signalflow_perlin_fade(fy);
    float rv = signalflow_perlin_lerp(signalflow_perlin_lerp(v00, v10, u),
                                      signalflow_perlin_lerp(v01, v11, u),
                                      v);

    /*--------------------------------------------------------------------------------
     * With unit gradients, 2D Perlin noise peaks at ±sqrt(0.5); scale to ±1.
     *--------------------------------------------------------------------------------*/
    return rv * M_SQRT2;
}

void PerlinNoise::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float x = this->x->out[channel][frame];
            float y = this->y->out[channel][frame];
            float detail = this->detail->out[channel][frame];
            float amplitude = 1.0;
            float amplitude_sum = 0.0;
            float rv = 0.0;

            for (int octave = 0; octave < this->num_octaves; octave++)
            {
                rv += this->get_noise(x, y) * amplitude;
                amplitude_sum += fabsf(amplitude);
                amplitude *= detail;
                x *= 2;
                y *= 2;
            }

            out[channel][frame] = rv / amplitude_sum;
        }
    }
}

}
