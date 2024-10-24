#pragma once

/**--------------------------------------------------------------------------------
 * @file ringbuffer.h
 * @brief RingBuffer is a single-producer, single-consumer ringbuffer.
 *
 *--------------------------------------------------------------------------------*/

#include <math.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum signalflow_interpolation_mode_t : unsigned int;

namespace signalflow
{

template <class T>
class RingBuffer
{
public:
    RingBuffer(unsigned int capacity);
    ~RingBuffer();

    void append(T value);
    void extend(T *ptr, unsigned int count);
    T get(double index);
    T operator[](double index) { return this->get(index); }

protected:
    T *data = nullptr;
    unsigned int capacity;
    unsigned int write_position;
    signalflow_interpolation_mode_t interpolation_mode;
};

template <class T>
class RingQueue : public RingBuffer<T>
{
public:
    RingQueue(unsigned int capacity)
        : RingBuffer<T>(capacity)
    {
        read_position = capacity - 256;
    }
    T pop();

private:
    unsigned int read_position;
};

template <class T>
RingBuffer<T>::RingBuffer(unsigned int capacity)
{
    if (capacity == 0)
    {
        throw std::runtime_error("RingBuffer must have a capacity greater than zero");
    }
    this->data = new T[capacity]();
    this->write_position = 0;
    this->capacity = capacity;
}

template <class T>
RingBuffer<T>::~RingBuffer()
{
    delete[] this->data;
}

template <class T>
void RingBuffer<T>::append(T value)
{
    this->data[this->write_position] = value;
    this->write_position = (this->write_position + 1) % this->capacity;
}

template <class T>
void RingBuffer<T>::extend(T *ptr, unsigned int count)
{
    for (int i = 0; i < count; i++)
        this->append(ptr[i]);
}

template <class T>
T RingBuffer<T>::get(double index)
{
    double frame = index + this->write_position;
    while (frame < 0)
    {
        frame += this->capacity;
    }
    frame = fmod(frame, this->capacity);

    double frame_frac = (frame - (int) frame);
    int frame_index = (int) frame;
    int next_frame_index = ((int) ceil(frame)) % this->capacity;

    T rv = ((1.0 - frame_frac) * this->data[frame_index]) + (frame_frac * this->data[next_frame_index]);

    return rv;
}

template <class T>
T RingQueue<T>::pop()
{
    T rv = this->data[this->read_position];
    this->read_position = (this->read_position + 1) % this->capacity;
    return rv;
}

}
