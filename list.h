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
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIST_NODE_T_DEFINED
struct node_t {
    struct node_t* previous;
    struct node_t* next;
    uint8_t data[0];
};
typedef struct node_t node_t;
#define LIST_NODE_T_DEFINED
#endif

typedef struct list_t {
    int size;
    size_t size_type;
    node_t* begin;
    node_t* end;
    int (*comparator)(const void*, const void*);
}list_t;


/**
  * @brief generate a list object that will contain objects of the specified size.
  * @param  data_size: size in bytes of the elements to be stored in the list
  * @return list that was just created
  */
list_t* list_create(size_t size_type);

/**
  * @brief free all memory allocated to the list and its elements.
  * @param  list: the list to be freed
  * @note list's value will be set to NULL after being freed
  */
void list_free(list_t** list);

/**
  * @brief clear and fres all elements of a list, without destroying the list itself.
  * @param  list: the list to clear
  */
void list_clear(list_t* list);

/**
  * @brief gets the n th element from a list
  * @param   list: the list to get the element from
  * @param   n: the 0-indexed element to get
  * @param   data: reference to the list data type. The value will be copied over
  * @return  0: success
  *          -1: failure
  * @warning do not iterate on a list using list_get. Complexity is O(n^2) 
  */
int list_get(list_t* list, int n, void* data);

/**
  * @brief add data to the end of the given list.
  * @param  list: the list to add the item to
  * @param  data: reference to the list's data type holding the value to be added
  * @return 0: success
  *         -1: failure
  */
int list_push_back(list_t* list, const void* data);

/**
  * @brief alias for list_push_back
  */
#define list_push(list, data) list_push_back(list, data)


/**
  * @brief remove the first item of the given list.
  * data is optional. A NULL value is acceptable.
  * @param  list: the list to remove the item from
  * @param  data: reference to the list's data type where the poped value will be copied 
  * @return 0: success
  *         -1: failure
  */
int list_pop_front(list_t* list, void* data);


/**
  * @brief remove the last item of the given list.
  * data is optional. A NULL value is acceptable.
  * @param  list: the list to remove the item from
  * @param  data: reference to the list's data type where the poped value will be copied
  * @return 0: success
  *         -1: failure
  */
int list_pop_back(list_t* list, void* data);

/**
 * @brief alias for list_pop_back
 */
#define list_pop(list, data) list_pop_back(list, data)

/**
  * @brief peek at the first item of the given list
  * @param  list: the list to peek at
  * @param  data: reference to the list's data type where the first value will be copied
  * @return 0: success
  *         -1: failure
  */
int list_peek_front(list_t* list, void* data);

/**
 * @brief alias for list_peek_front
 */
#define list_peek(list, data) list_peek_front(list, data)


/**
  * @brief peek at the last item of the given list
  * @param  list: the list to peek at
  * @param  data: reference to the list's data type where the first value will be copied
  * @return 0: success
  *         -1: failure
  */
int list_peek_back(list_t* list, void* data);


/**
 * @brief set the list's comparator that can be used for operations such as sorting
 * @param  list: the list to set the comparator to
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
 */
int list_set_comparator(list_t* list, int (*comp)(const void*, const void*));

/**
 * @brief sorts the given list according to its internal comparator
 * @param   list: the list to sort
 * @warning list_set_comparator must be called prior to calling list_sort
 * @return  0: success
 *          -1: failure
 * @see list_set_comparator
 */
int list_sort(list_t* list);

/**
 * @brief sorts the given list according to the comparator passed as argument
 * @param   list: the list to sort
 * @param   comp: a standard comparator function
 * @return  0: success
 *          -1: failure
 */
int list_sort_with(list_t* list, int(*comp)(const void*, const void*));

/**
  * @brief check if the list contains data
  * @param      list: list to perform the operation on
  * @param      data: reference to the list's data type holding the value to be found
  * @return     true: data's value is found
  *             false: otherwise
  * @warning    list_set_comparator must be called prior to calling list_contains
  * @see list_set_comparator
  */
bool list_contains(list_t* list, const void* data);

/**
 * @brief add data to the list while respecting the list's order
 * In case the list should be ordered at all time, this should be the sole function used to insert
 * nodes to the list. Calling list_add_ordered will ensure that all elements in the lists are naturally
 * sorted without having to perform a sorting operation. 
 * @param  list: the list to add the item to
 * @param  data: reference to the list's data type holding the value to be added
 * @return 0: success
 *         -1: failure
 * @warning    list_set_comparator must be called prior to calling list_contains
 * @see list_set_comparator
 */
int list_add_ordered(list_t* list, const void* data);

#ifdef __cplusplus
}
#endif

#endif