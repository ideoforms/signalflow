/*--------------------------------------------------------------------*
 * util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#include "signalflow/buffer/buffer.h"
#include "signalflow/core/util.h"

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

namespace signalflow
{

/*--------------------------------------------------------------------*
 * timestamp(): Return microsecond-accurate timestamp.
 * Useful for monitoring function runtimes.
 *--------------------------------------------------------------------*/
double signalflow_timestamp()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (tv.tv_usec / 1000000.0);
}

/*--------------------------------------------------------------------*
 * signalflow_clip(): Constrain a value within two bounds.
 *--------------------------------------------------------------------*/
double signalflow_clip(double value, double min, double max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
    return value;
}

/*--------------------------------------------------------------------*
 * signalflow_wrap(): Wrap a value within two bounds.
 *--------------------------------------------------------------------*/
double signalflow_wrap(double value, double min, double max)
{
    double range = max - min;
    double remainder = fmodf((value - min), range);
    return min + remainder;
}

double signalflow_fold(double value, double min, double max)
{
    double range = max - min;
    double remainder = fmodf((value - min), (range * 2));
    if (remainder > range)
    {
        return min + 2 * range - remainder;
    }
    else
    {
        return min + remainder;
    }
}

/*--------------------------------------------------------------------*
 * signalflow_scale_lin_lin(): Map a value onto a linear range.
 *--------------------------------------------------------------------*/
double signalflow_scale_lin_lin(double value, double fromA, double fromB, double toA, double toB)
{
    double norm = (value - fromA) / (fromB - fromA);
    return toA + norm * (toB - toA);
}

/*--------------------------------------------------------------------*
 * scale_lin_exp(): Map a value onto an exponential range.
 *--------------------------------------------------------------------*/
double signalflow_scale_lin_exp(double value, double a, double b, double c, double d)
{
    if (value <= a)
        return c;
    if (value >= b)
        return d;
    return pow(d / c, (value - a) / (b - a)) * c;
}

/*--------------------------------------------------------------------*
 * signalflow_scale_exp_lin(): Map a value from an exponential to linear range.
 *--------------------------------------------------------------------*/
double signalflow_scale_exp_lin(double value, double a, double b, double c, double d)
{
    if (value <= a)
        return c;
    if (value >= b)
        return d;
    return (log(value / a)) / (log(b / a)) * (d - c) + c;
}

float signalflow_frequency_to_midi_note(float frequency)
{
    /*--------------------------------------------------------------------*
     * signalflow_frequency_to_midi_note(): Map a frequency value onto a MIDI note index.
     * 440 = A4 = 69
     *--------------------------------------------------------------------*/
    return 69.0 + 12 * log2(frequency / 440.0);
}

float signalflow_midi_note_to_frequency(float midi)
{
    /*--------------------------------------------------------------------*
     * signalflow_midi_note_to_frequency(): Map a MIDI note to a frequency value.
     * 440 = A4 = 69
     *--------------------------------------------------------------------*/
    return 440.0 * powf(2, (midi - 69) / 12.0);
}

float signalflow_db_to_amplitude(float db)
{
    return powf(10.0f, db * 0.05f);
}

float signalflow_amplitude_to_db(float amp)
{
    amp = MAX(amp, 1e-9);
    amp = MIN(amp, 1e9);
    return 20.0f * log10f(amp);
}

void signalflow_save_block_to_text_file(sample *block, int num_samples, std::string filename)
{
    FILE *fd = fopen(filename.c_str(), "w");
    if (!fd)
    {
        throw std::runtime_error("Couldn't open file for write: " + filename);
    }
    for (int i = 0; i < num_samples; i++)
    {
        fprintf(fd, "%f\n", block[i]);
    }
    fclose(fd);
}

void signalflow_save_block_to_wav_file(sample *block, int num_samples, std::string filename)
{
    BufferRef buf = new Buffer(1, num_samples, &block);
    buf->save(filename);
}

} /* namespace signalflow */
