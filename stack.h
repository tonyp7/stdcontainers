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

@file stack.h
@author Tony Pottier
@brief Defines a standard stack, a subset of the vector.h implementation.

@see https://github.com/tonyp7/stdcontainers

*/

#ifndef _STACK_H_
#define _STACK_H_

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef vector_t stack_t;

int (*stack_create)(stack_t* stack, size_t size_type) = vector_create;

int (*stack_push)(stack_t* stack, const void* data) = vector_push_back;

int (*stack_pop)(stack_t* stack, void* data) = vector_pop_back;

void* (*stack_peek)(stack_t* stack) = vector_back;

void (*stack_clear)(stack_t* stack) = vector_clear;

#ifdef __cplusplus
}
#endif

#endif