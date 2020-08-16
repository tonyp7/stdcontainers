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
#define VECTOR_MINIMUM_CAPACITY 2

/**
  * @brief initialize an empty vector with an initial capacity of VECTOR_DEFAULT_INITIAL_SIZE
  * @param      vector: pointer to the vector_t struct to be initialized
  * @param		size_type: size in bytes of the elements to be stored
  * @return		0: success
  *				-1: failure
  */
int vector_create(vector_t* vector, size_t size_type);

/**
  * @brief initialize an empty vector with the specified initial capacity
  * @param      vector: pointer to the vector_t struct to be initialized
  * @param		size_type: size in bytes of the elements to be stored
  * @param		capacity: initial capacity in number of elements that will be allocated
  * @return		0: success
  *				-1: failure
  */
int vector_create_with(vector_t* vector, size_t size_type, size_t capacity);


/**
  * @brief clears all elements of the vector
  * @param  vector: the vector to perform the operation on
  */
void vector_clear(vector_t* vector);

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

/**
  * @brief add data to the end of the vector
  * @param		vector: the vector to perform the operation on
  * @param		data: reference to the vector's data type holding the value to be added
  * @return		0: success
  *				-1: failure
  */
int vector_push_back(vector_t* vector, const void* data);

/**
  * @brief add data to the beginning of the vector
  * @param		vector: the vector to perform the operation on
  * @param		data: reference to the vector's data type holding the value to be added
  * @return		0: success
  *				-1: failure
  * @note inserting values at the beginning or at a random location requires
  * shifting the entire underlying array.
  */
int vector_push_front(vector_t* vector, const void* data);

/**
  * @brief add data at a the specified index
  * @param		vector: the vector to perform the operation on
  * @param		n: the 0 indexed n th value
  * @param		data: reference to the vector's data type holding the value to be added
  * @return		0: success
  *				-1: failure
  * @note data after the specified index will be shifted right 
  */
int vector_insert(vector_t* vector, int n, const void* data);

/**
  * @brief assign the given value to the data located at the specified index
  * @param		vector: the vector to perform the operation on
  * @param		n: the 0 indexed n th value
  * @param		data: reference to the vector's data type
  * @return		0: success
  *				-1: failure
  */
int vector_assign(vector_t* vector, int n, const void* data);

/**
  * @brief remove the last value of the given vector
  * data is optional. A NULL value is acceptable.
  * @param		vector: the vector to perform the operation on
  * @param		data: reference to the list's data type where the poped value will be copied
  * @return		0: success
  *				-1: failure
  */
int vector_pop_back(vector_t* vector, void* data);

/**
  * @brief remove the first value of the given vector
  * data is optional. A NULL value is acceptable.
  * @param		vector: the vector to perform the operation on
  * @param		data: reference to the list's data type where the poped value will be copied
  * @return		0: success
  *				-1: failure
  * @note the entire underlying array will be shifted left. vector_pop_front can be a costly operation.
  */
int vector_pop_front(vector_t* vector, void* data);

/**
  * @brief remove the n th element from the vector
  * @param		vector: the vector to perform the operation on
  * @param		n: the 0 indexed n th value
  * @return		0: success
  *				-1: failure
  * @note the elements after the specified index will be shifted left. vector_erase can be a costly operation.
  */
int vector_erase(vector_t* vector, int n);

/**
  * @brief change the vector's capacity to match the exact number of elements it contains
  * @param		vector: the vector to perform the operation on
  * @return		0: success
  *				-1: failure
  */
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