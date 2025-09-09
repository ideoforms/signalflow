/*--------------------------------------------------------------------*
 * random.h: Random number generation
 *--------------------------------------------------------------------*/

#pragma once

namespace signalflow
{

/*--------------------------------------------------------------------*
 * Implementation of Blackman and Vigna's Xoshiro RNG, benchmarked
 * as ~30% faster than std::random's Mersenne twister (Mac M1)
 *--------------------------------------------------------------------*/
struct xoshiro256ss
{
    typedef unsigned long long u64;
    u64 s[4];

    // SplitMix64 for seeding
    static u64 splitmix64(u64 &x)
    {
        u64 z = (x += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }

    // Default constructor seeds with 0
    xoshiro256ss() { seed(0); }

    // Constructor with seed
    explicit xoshiro256ss(u64 new_seed) { seed(new_seed); }

    // Reseed the RNG
    void seed(u64 seed)
    {
        s[0] = splitmix64(seed);
        s[1] = splitmix64(seed);
        s[2] = splitmix64(seed);
        s[3] = splitmix64(seed);
    }

    // Generator interface for std::random_distribution
    typedef u64 result_type;
    static constexpr u64 min() { return 0ULL; }
    static constexpr u64 max() { return 0xFFFFFFFFFFFFFFFFULL; }

    // Rotate left
    static u64 rotl(u64 x, int k)
    {
        return (x << k) | (x >> (64 - k));
    }

    // Generate next number
    u64 operator()()
    {
        u64 result = rotl(s[1] * 5, 7) * 9;
        u64 t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }
};

void random_init();
void random_seed(long seed);

double random_gaussian(double mean, double sd);
double random_gaussian();
double random_uniform();
double random_uniform(double from, double to);
unsigned long random_integer(unsigned long from, unsigned long to);
bool random_coin(double limit);
void random_shuffle(int *values, int count);
float random_exponential(float from, float to);
float random_exponential_dist(float mu);

}
