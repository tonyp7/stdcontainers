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

@file deque.h
@author Tony Pottier
@brief Defines a standard double-ended queue (deque), a subset of the list.h implementation.

@see https://github.com/tonyp7/stdcontainers

*/

#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "list.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef list_t deque_t;

deque_t* (*deque_create)(size_t size_type) = list_create;

int (*deque_push_back)(deque_t* deque, const void* data) = list_push_back;

int (*deque_push_front)(deque_t* deque, const void* data) = list_push_front;

int (*deque_pop_front)(deque_t* deque, void* data) = list_pop_front;

int (*deque_pop_back)(deque_t* deque, void* data) = list_pop_back;

int (*deque_peek_front)(deque_t* deque, void* data) = list_peek_front;

int (*deque_peek_back)(deque_t* deque, void* data) = list_peek_back;

void (*deque_free)(deque_t** deque) = list_free;

#ifdef __cplusplus
}
#endif

#endif
