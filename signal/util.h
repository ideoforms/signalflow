/*--------------------------------------------------------------------*
 * Util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#pragma once

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

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

	double timestamp();

	double clip(double value, double min, double max);
	double map(double value, double fromA, double fromB, double toA, double toB);
	double linexp (double value, double a, double b, double c, double d);
	double explin (double value, double a, double b, double c, double d);

	float freq_to_midi(float frequency);
	float midi_to_freq(float midi);
}
