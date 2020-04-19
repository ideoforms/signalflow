/*--------------------------------------------------------------------*
 * Util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#pragma once

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

namespace libsignal
{

    double timestamp();

    double clip(double value, double min, double max);
    double map(double value, double fromA, double fromB, double toA, double toB);
    double linexp (double value, double a, double b, double c, double d);
    double explin (double value, double a, double b, double c, double d);

    float freq_to_midi(float frequency);
    float midi_to_freq(float midi);

}
