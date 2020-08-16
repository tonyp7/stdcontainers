/**
Copyright (c) 2020 Tony Pottier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

@file vector.c
@author Tony Pottier
@brief Source code for a standard vector (dynamic array) container

The vector will double its capacity when it can no longer hold data, and is
lazier when it comes to freeing memory: the vector size needs to be 1/4 of
its capacity before it starts reallocating.
This behavior should lead to better performance. Memory conscious people
can use the shrink_to_fit API

@see https://github.com/tonyp7/stdcontainers

*/

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

static inline int _vector_resize(vector_t* vector, size_t new_capacity)
{
	void* new_data = realloc(vector->data, new_capacity * vector->size_type);

	if (new_data) {
		vector->data = new_data;
		vector->capacity = new_capacity;
	}
	return new_data != NULL;
}


static inline int _vector_shrink_check(vector_t* vector)
{
	return vector->size <= vector->capacity >> 2;
}

static inline int _vector_shrink(vector_t* vector)
{
	size_t new_capacity = vector->capacity >> 2;
	if (new_capacity < VECTOR_MINIMUM_CAPACITY) {
		new_capacity = VECTOR_MINIMUM_CAPACITY;
	}
	return _vector_resize(vector, VECTOR_MINIMUM_CAPACITY);
}

static inline void* _vector_at(vector_t* vector, int n)
{
	return vector->data + (n * vector->size_type);
}

static inline void _vector_assign_ptr(vector_t* vector, void* dst, const void* data)
{
	memcpy(dst, data, vector->size_type);
}

static inline void _vector_assign(vector_t* vector, int n, const void* data)
{
	_vector_assign_ptr(vector, _vector_at(vector, n), data);
}

static inline int _vector_grow_check(vector_t* vector)
{
	return vector->size >= vector->capacity;
}

static inline int _vector_grow(vector_t* vector)
{
	return _vector_resize(vector, vector->capacity << 1);
}

static inline int _vector_shift_right(vector_t* vector, int n)
{
	void* src = _vector_at(vector, n);
	void* dst = _vector_at(vector, n + 1);

	memmove(dst, src, (vector->size - n) * vector->size_type);

	return 0;
}


static inline int _vector_shift_left(vector_t* vector, int n)
{
	void* dst = _vector_at(vector, n);
	void* src = _vector_at(vector, n + 1);

	memmove(dst, src, (vector->size - n - 1) * vector->size_type);

	return 0;
}


vector_t* vector_create(size_t size_type)
{
	return vector_create_with(size_type, VECTOR_DEFAULT_INITIAL_SIZE);
}

vector_t* vector_create_with(size_t size_type, size_t capacity)
{
	vector_t* vector = malloc(sizeof(vector_t) + sizeof(uint8_t) * (capacity * size_type) );

	if (!vector) return NULL;
	if (capacity <= 0) capacity = VECTOR_DEFAULT_INITIAL_SIZE;

	vector->capacity = capacity;
	vector->size_type = size_type;
	vector->size = 0;

	vector->data = malloc(capacity * size_type);

	if (!vector->data) {
		free(vector);
		vector = NULL;
	}

	return vector;
}


void vector_clear(vector_t* vector)
{
	if (vector->size) {
		vector->size = 0;

		if (_vector_shrink_check(vector)) {
			_vector_shrink(vector);
		}
	}
}

void vector_free(vector_t** vector)
{
	if (*vector) {

		free((*vector)->data);
		free(*vector);

		*vector = NULL;
	}
}



void* vector_at(vector_t* vector, int n)
{
	return _vector_at(vector, n);
}

int vector_sort(vector_t* vector, int (*comp)(const void*, const void*))
{
	qsort(vector->data, vector->size, vector->size_type, comp);
	return 0;
}

int vector_push_back(vector_t* vector, const void* data)
{
	if (_vector_grow_check(vector)) {
		if (!_vector_grow(vector)) {
			return -1;
		}
	}

	_vector_assign(vector, vector->size, data);

	vector->size++;

	return 0;
}

int vector_push_front(vector_t* vector, const void* data) 
{
	return vector_insert(vector, 0, data);
}

int vector_insert(vector_t* vector, int n, const void* data)
{
	if (_vector_grow_check(vector)) {
		if (!_vector_grow(vector)) {
			return -1;
		}
	}

	_vector_shift_right(vector, n);
	_vector_assign(vector, n, data);

	vector->size++;

	return 0;
}

int vector_assign(vector_t* vector, int n, const void* data)
{
	_vector_assign(vector, n, data);
	return 0;
}

int vector_pop_back(vector_t* vector, void* data)
{
	if (vector->size == 0) return -1;

	/* optional: get the pop'd data back */
	if (data) {
		void* src = _vector_at(vector, vector->size - 1);
		_vector_assign_ptr(data, src, vector->size_type);
	}

	vector->size--;

	if (_vector_shrink_check(vector)) {
		_vector_shrink(vector);
	}

	return 0;

}


int vector_pop_front(vector_t* vector, void* data)
{
	if (vector->size == 0) return -1;

	/* optional: get the pop'd data back */
	if (data) {
		_vector_assign_ptr(data, vector->data, vector->size_type);
	}

	_vector_shift_left(vector, 0);

	vector->size--;

	if (_vector_shrink_check(vector)) {
		_vector_shrink(vector);
	}

	return 0;

}

int vector_erase(vector_t* vector, int n)
{
	if (vector->size == 0 || n >= vector->size) return -1;

	_vector_shift_left(vector, n);

	vector->size--;

	if (_vector_shrink_check(vector)) {
		_vector_shrink(vector);
	}

	return 0;

}


void* vector_front(vector_t* vector)
{
	if (vector->size) {
		return _vector_at(vector, 0);
	}
	else {
		return NULL;
	}
	
}

void* vector_back(vector_t* vector)
{
	if (vector->size) {
		return _vector_at(vector, vector->size - 1);
	}
	else {
		return NULL;
	}
	
}


int vector_shrink_to_fit(vector_t* vector)
{
	if (vector->size && vector->size != vector->capacity) {
		
		if (_vector_resize(vector, vector->size)) {
			return 0;
		}
		else {
			return -1;
		}
	}
	return -1;

}