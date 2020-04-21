/*--------------------------------------------------------------------*
 * random.h: Random number generation
 *--------------------------------------------------------------------*/

#pragma once

namespace libsignal
{

void random_init();
void random_seed(long seed);

double random_gaussian(double mean, double sd);
double random_gaussian();
double random_uniform();
double random_uniform(double to);
double random_uniform(double from, double to);
unsigned long random_integer(unsigned long to);
unsigned long random_integer(unsigned long from, unsigned long to);
bool random_coin(double limit);
void random_shuffle(int *values, int count);

float random_exponential(float mu);
float random_cauchy(float a);
float random_beta(float a, float b);
float random_gamma(float a, float b);
float random_levy(float c, float alpha);

float random_exponential_pdf(float x, float mu);
float random_cauchy_pdf(float x, float a);
float random_beta_pdf(float x, float a, float b);
float random_gamma_pdf(float x, float a, float b);

}
