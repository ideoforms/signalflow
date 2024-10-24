#pragma once

/**--------------------------------------------------------------------------------
 * @file ringbuffer.h
 * @brief RingBuffer is a single-producer, single-consumer ringbuffer.
 *
 *--------------------------------------------------------------------------------*/

#include <math.h>
#include <mutex>
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
    void extend(std::vector<T> vec);
    T get(double index);
    T operator[](double index) { return this->get(index); }
    unsigned int get_capacity() { return this->capacity; }
    unsigned int get_write_position() { return this->write_position; }

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
        this->read_position = this->capacity - 1;
        this->filled_count = 0;
    }
    T pop();

    int get_filled_count() { return this->filled_count; }
    void append(T value);

private:
    unsigned int read_position;
    int filled_count;
    std::mutex mutex;
};

template <class T>
RingBuffer<T>::RingBuffer(unsigned int capacity)
{
    if (capacity == 0)
    {
        throw std::runtime_error("RingBuffer must have a capacity greater than zero");
    }
    this->data = new T[capacity]();
    this->write_position = capacity - 1;
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
    this->write_position = (this->write_position + 1) % this->capacity;
    this->data[this->write_position] = value;
}

template <class T>
void RingBuffer<T>::extend(T *ptr, unsigned int count)
{
    for (int i = 0; i < count; i++)
        this->append(ptr[i]);
}

template <class T>
void RingBuffer<T>::extend(std::vector<T> vec)
{
    for (auto item : vec)
        this->append(item);
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
    mutex.lock();
    this->read_position = (this->read_position + 1) % this->capacity;
    this->filled_count--;
    T rv = this->data[this->read_position];
    mutex.unlock();
    return rv;
}

template <class T>
void RingQueue<T>::append(T value)
{
    mutex.lock();
    this->RingBuffer<T>::append(value);
    this->filled_count++;
    mutex.unlock();
}

}
