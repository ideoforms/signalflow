/*--------------------------------------------------------------------*
 * Util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#pragma once

#include "signal/core/constants.h"

#include <stdexcept>
#include <string>

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)
#endif

namespace libsignal
{

double timestamp();

double clip(double value, double min, double max);
double map(double value, double fromA, double fromB, double toA, double toB);
double linexp(double value, double a, double b, double c, double d);
double explin(double value, double a, double b, double c, double d);

float freq_to_midi(float frequency);
float midi_to_freq(float midi);

float signal_db_to_amp(float db);
float signal_amp_to_db(float amp);

void signal_save_block_to_text_file(sample *block, int num_samples, std::string filename);
void signal_save_block_to_wav_file(sample *block, int num_samples, std::string filename);

}
