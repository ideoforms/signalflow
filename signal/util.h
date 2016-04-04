/*--------------------------------------------------------------------*
 * Util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#pragma once

#define MAX(a, b) (a > b ? a : b)


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

	double timestamp();

	double clip(double value, double min, double max);
	double map(double value, double fromA, double fromB, double toA, double toB);
	double linexp (double value, double a, double b, double c, double d);
	double explin (double value, double a, double b, double c, double d);

	float freq_to_midi(float frequency);
	float midi_to_freq(float midi);
}
