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

double signal_timestamp();

double signal_clip(double value, double min, double max);
double signal_scale_lin_lin(double value, double fromA, double fromB, double toA, double toB);
double signal_scale_lin_exp(double value, double a, double b, double c, double d);
double signal_scale_exp_lin(double value, double a, double b, double c, double d);

float signal_frequency_to_midi_note(float frequency);
float signal_midi_note_to_frequency(float midi);

float signal_db_to_amp(float db);
float signal_amp_to_db(float amp);

void signal_save_block_to_text_file(sample *block, int num_samples, std::string filename);
void signal_save_block_to_wav_file(sample *block, int num_samples, std::string filename);

}
