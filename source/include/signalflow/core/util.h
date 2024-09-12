/*--------------------------------------------------------------------*
 * Util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#pragma once

#include "signalflow/core/constants.h"

#include <stdexcept>
#include <string>
#include <vector>

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)
#endif

namespace signalflow
{
double signalflow_timestamp();
long signalflow_create_random_seed();

double signalflow_clip(double value, double min, double max);
double signalflow_wrap(double value, double min, double max);
double signalflow_fold(double value, double min, double max);
double signalflow_interpolate_linear(double a, double b, double xfade);
double signalflow_interpolate_equal_power(double a, double b, double xfade);

double signalflow_scale_lin_lin(double value, double fromA, double fromB, double toA, double toB);
double signalflow_scale_lin_exp(double value, double a, double b, double c, double d);
double signalflow_scale_exp_lin(double value, double a, double b, double c, double d);

float signalflow_frequency_to_midi_note(float frequency);
float signalflow_midi_note_to_frequency(float midi);

float signalflow_db_to_amplitude(float db);
float signalflow_amplitude_to_db(float amp);

void signalflow_save_block_to_text_file(sample *block, int num_samples, std::string filename);
void signalflow_save_block_to_wav_file(sample *block, int num_samples, std::string filename);

float signalflow_array_sum(float *array, size_t size);
float signalflow_array_mean(float *array, size_t size);

float signalflow_calculate_decay_coefficient(float decay_time,
                                             unsigned int sample_rate,
                                             float decay_level);

std::vector<int> signalflow_binary_sequence_to_vector(std::string binary);

}
