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

@file queue.h
@author Tony Pottier
@brief Defines a standard FIFO queue, a subset of the list.h implementation

@see https://github.com/tonyp7/stdcontainers

*/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef list_t queue_t;

/**
  * @brief generate a queue object that will contain objects of the specified size.
  * @param  data_size: size in bytes of the elements to be stored in the queue
  * @return queue that was just created
  */
int (*queue_create)(queue_t* queue, size_t size_type) = list_create;

node_t* (*queue_push)(queue_t* queue, const void* data) = list_push_back;

int (*queue_pop)(queue_t* queue, void* data) = list_pop_front;

node_t* (*queue_peek_front)(queue_t* queue, void* data) = list_peek_front;

node_t* (*queue_peek_back)(queue_t* queue, void* data) = list_peek_back;

void (*queue_clear)(queue_t* queue) = list_clear;

#ifdef __cplusplus
}
#endif

#endif