#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "signal/core/constants.h"

enum signal_interpolation_mode_t : unsigned int;

namespace libsignal
{

template <class T>
class RingBuffer
{
public:
    RingBuffer(int size);
    ~RingBuffer();

    void append(T value);
    void extend(T *ptr, int count);
    T get(double index);
    T operator[](double index) { return this->get(index); }

private:
    T *data = nullptr;
    int size;
    int position;
    signal_interpolation_mode_t interpolation_mode;
};

template <class T>
RingBuffer<T>::RingBuffer(int size)
{
    this->data = new T[size]();
    this->position = 0;
    this->size = size;
}

template <class T>
RingBuffer<T>::~RingBuffer()
{
    delete [] this->data;
}

template <class T>
void RingBuffer<T>::append(T value)
{
    this->data[this->position] = value;
    this->position = (this->position + 1) % this->size;
}

template <class T>
void RingBuffer<T>::extend(T *ptr, int count)
{
    for (int i = 0; i < count; i++)
        this->append(ptr[i]);
}

template <class T>
T RingBuffer<T>::get(double index)
{
    double frame = index + this->position;
    while (frame < 0)
    {
        frame += this->size;
    }
    frame = fmod(frame, this->size);

    double frame_frac = (frame - (int) frame);
    T rv = ((1.0 - frame_frac) * data[(int) frame]) + (frame_frac * data[(int) ceil(frame)]);

    return rv;
}

}
