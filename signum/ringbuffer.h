#pragma once

#include <stdlib.h>
#include <string.h>

template <class T>
class RingBuffer
{
	public:
		RingBuffer(int size);
		~RingBuffer();

		void append(T value);
		void extend(T *ptr, int count);
		T get(int index);
		T operator [](int index){ return this->get(index); }

	private:
		T *data;
		int size;
		int position;
};


template <class T>
RingBuffer<T>::RingBuffer(int size)
{
	this->data = (T *) malloc(sizeof(T) * size);
	memset(this->data, 0, sizeof(T) * size);
	this->position = 0;
	this->size = size;
}

template <class T>
RingBuffer<T>::~RingBuffer()
{
	free(this->data);
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
T RingBuffer<T>::get(int index)
{
	int new_index = index + this->position - 1;
	new_index = (new_index % this->size);
	return data[new_index];
}

