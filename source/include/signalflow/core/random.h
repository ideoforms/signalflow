/*--------------------------------------------------------------------*
 * random.h: Random number generation
 *--------------------------------------------------------------------*/

#pragma once

namespace signalflow
{

void random_init();
void random_seed(long seed);

double random_gaussian(double mean, double sd);
double random_gaussian();
double random_uniform();
double random_uniform(double from, double to);
unsigned long random_integer(unsigned long from, unsigned long to);
bool random_coin(double limit);
void random_shuffle(int *values, int count);
float random_exponential(float mu);

}
