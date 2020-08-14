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

@file list.h
@author Tony Pottier
@brief Doubly linked list Standard C Container

@see https://github.com/tonyp7/stdcontainers

*/

#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct node_t {
    struct node_t* previous;
    struct node_t* next;
    uint8_t data[0];
};
typedef struct node_t node_t;

typedef struct list_t {
    int size;
    size_t data_size;
    node_t* first;
    node_t* last;
}list_t;


/**
 * @brief generate a list object that will contain objects of size_t data_size
 */
list_t* list_create(size_t data_size);

/**
 * @brief frees all memory allocated to the list and its elements
 */
void list_free(list_t* list);

/**
 * @brief clear and frees all elements of a list, without destroying the list itself
 */
void list_clear(list_t* list);

/**
 * @brief gets the n th element from a list
 * @param list the list to get the element from
 * @param n the 0-indexed element to get
 * @param data reference to an element of the list data type that will get the selected element
 * @return 0 on success, -1 otherwise
 * @warning do not iterate on a list using list_get. Complexity is O(n^2) 
 */
int list_get(list_t* list, int n, void* data);

/**
 * @brief add data to the end of the list
 */
int list_push_back(list_t* list, void* data);

/**
 * @brief alias for list_push_back
 */
#define list_push(list, data) list_push_back(list, data)

/**
 * @brief add data in the list as per the comparison function provided
 */
int list_add_ordered(list_t* list, void* data, int (*comp)(void*, void*));


/**
 * @brief Remove an item from the beginning of the list
 */
int list_pop_front(list_t* list, void* data);


/**
 * @brief Remove an item at the end of the list
 */
int list_pop_back(list_t* list, void* data);

/**
 * @brief alias for list_pop_back
 */
#define list_pop(list, data) list_pop_back(list, data)

/**
 * @brief Get the first element of the list
 */
int list_peek_front(list_t* list, void* data);

/**
 * @brief alias for list_peek_front
 */
#define list_peek(list, data) list_peek_front(list, data)


int list_peek_front(list_t* list, void* data);


int list_sort(list_t* list, int (*comp)(void*, void*));

#ifdef __cplusplus
}
#endif

#endif