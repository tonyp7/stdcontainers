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

@see https://github.com/tonyp7/stdcontainers

*/

#include <stdint.h>
#include <stdlib.h>
#include "vector.h"

vector_t* vector_create(size_t size_type, size_t capacity)
{
	vector_t* vector = malloc(sizeof(vector_t) + sizeof(uint8_t) * (capacity * size_type) );

	if (!vector) return NULL;

	vector->capacity = capacity;
	vector->size_type = size_type;
	vector->size = 0;

	return vector;
}

void* vector_at(vector_t* vector, int n)
{

}

int vector_sort(vector_t* vector, int (*comp)(const void*, const void*))
{

}

int vector_push_back(vector_t* vector, const void* data)
{
	if (vector->size >= vector->capacity) {
		/* vector needs to be realloc'd */
	}
}

int vector_insert(vector_t* vector, int n, const void* data)
{

}

int vector_pop_back(vector_t* vector, void* data)
{

}

void* vector_front(vector_t* vector)
{

}

void* vector_back(vector_t* vector)
{

}