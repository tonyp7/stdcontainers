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

@file forward_list.h
@author Tony Pottier
@brief Singly Linked list Standard C Container

@see https://github.com/tonyp7/stdcontainers

*/

#ifndef _FORWARD_LIST_H_
#define _FORWARD_LIST_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4200) /* C4200: nonstandard extension used: zero-sized array in struct/union */ 
#endif
    struct forward_node_t {
        struct forward_node_t* next;
        uint8_t data[0];
    };
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

typedef struct forward_node_t forward_node_t;

typedef struct forward_list_t {
    int size;
    size_t size_type;
    forward_node_t* begin;
    forward_node_t* end;
    int (*comparator)(const void*, const void*);
}forward_list_t;


/*****************************/
/* constructor/destructor    */
/*****************************/

int forward_list_create(forward_list_t* list, size_t size_type);
void forward_list_clear(forward_list_t* list);
void forward_list_destroy(forward_list_t* list);

/*********************/
/* element access    */
/*********************/

void* forward_list_at(forward_list_t* list, int n);
void* forward_list_front(forward_list_t* list);
void* forward_list_back(forward_list_t* list);


/*********************/
/* insertion         */
/*********************/

int forward_list_assign(forward_list_t* vector, int n, const void* data);
forward_node_t* forward_list_push_front(forward_list_t* list, const void* data);
forward_node_t* forward_list_push_back(forward_list_t* list, const void* data);
forward_node_t* forward_list_add_ordered(forward_list_t* list, const void* data);
forward_node_t* forward_list_insert_after(forward_list_t*, forward_node_t* node, void* data);


/*********************/
/* deletion          */
/*********************/

int forward_list_pop_front(forward_list_t* list, void* data);
int forward_list_erase(forward_list_t*, int n);
int forward_list_erase_after(forward_list_t*, forward_node_t* node, void* data);


/*********************/
/* compare           */
/*********************/

int forward_list_set_comparator(forward_list_t* list, int (*comp)(const void*, const void*));
forward_node_t* forward_list_contains(forward_list_t* list, const void* data);




#ifdef __cplusplus
}
#endif

#endif

