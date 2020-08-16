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

@file vector.h
@author Tony Pottier
@brief Defines a standard vector (dynamic array) container

@see https://github.com/tonyp7/stdcontainers

*/

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vector_t {
	size_t size;
	size_t capacity;
	size_t size_type;
	uint8_t* data;
}vector_t;


#define VECTOR_DEFAULT_INITIAL_SIZE 2

/**
  * @brief create an empty vector with an initial capacity of VECTOR_DEFAULT_INITIAL_SIZE
  * @param		size_type: size in bytes of the elements to be stored
  * @return		vector_t*: success
  *				NULL: failure
  */
vector_t* vector_create(size_t size_type);

/**
  * @brief create an empty vector with an initial capacity of VECTOR_DEFAULT_INITIAL_SIZE
  * @param		size_type: size in bytes of the elements to be stored
  * @param		capacity: initial capacity in number of elements that will be allocated
  * @return		vector_t*: success
  *				NULL: failure
  */
vector_t* vector_create_with(size_t size_type, size_t capacity);

/**
  * @brief free all memory allocated to the vector and its elements.
  * @param  vector: the vector to perform the operation on
  * @note vector's value will be set to NULL after being freed
  */
void vector_free(vector_t** vector);

/**
 * @brief sort the given vector according to the comparator passed as argument
 * @param  vector: the vector to perform the operation on
 * @param  comp: a standard comparator function
 * @return 0: success
 *         -1: failure
 * @code{c}
 * // A typical comparator for integers would be defined as such:
 * int int_comparator(const void* a, const void* b)
 * {
 *    return *((int*)a) - *((int*)b);
 * }
 * @endcode
 * @note internally, vector_sort will call stdlib's quick sort implementation
 */
int vector_sort(vector_t* vector, int (*comp)(const void*, const void*));

int vector_push_back(vector_t* vector, const void* data);

int vector_push_front(vector_t* vector, const void* data);

int vector_insert(vector_t* vector, int n, const void* data);

int vector_assign(vector_t* vector, int n, const void* data);

int vector_pop_back(vector_t* vector, void* data);

int vector_pop_front(vector_t* vector, void* data);

int vector_erase(vector_t* vector, int n);

int vector_shrink_to_fit(vector_t* vector);

/**
  * @brief access the vector's first value
  * @param		vector: the vector to perform the operation on
  * @return		void*: pointer to the data
  *				NULL: failure
  */
void* vector_front(vector_t* vector);

/**
  * @brief access the vector's last value
  * @param		vector: the vector to perform the operation on
  * @return		void*: pointer to the data
  *				NULL: failure
  */
void* vector_back(vector_t* vector);

/**
  * @brief access the vector's nth value
  * @param		vector: the vector to perform the operation on
  * @param		n: the 0 indexed n th value
  * @return		void*: pointer to the data
  *				NULL: failure
  */
void* vector_at(vector_t* vector, int n);



#ifdef __cplusplus
}
#endif


#endif