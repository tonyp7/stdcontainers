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

@file forward_list.c
@author Tony Pottier
@brief Singly Linked list Standard C Container

@see https://github.com/tonyp7/stdcontainers

*/


#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "forward_list.h"


inline forward_node_t* _forward_list_node_at(forward_list_t* list, int n)
{
	int i = 0;
	forward_node_t* node = list->begin;
	while (i != n) {
		node = node->next;
		++i;
	}
	return node;
}

inline forward_node_t* _forward_node_alloc(forward_list_t* list)
{
	return (forward_node_t*)malloc(sizeof(forward_node_t) + sizeof(uint8_t) * list->size_type);
}

inline forward_node_t* _forward_node_alloc_and_assign(forward_list_t* list, void* data)
{
	forward_node_t* new_node = (forward_node_t*)malloc(sizeof(forward_node_t) + sizeof(uint8_t) * list->size_type);
	if (!new_node) return NULL; /* memory alloc error */

	memcpy(new_node->data, data, list->size_type);

	return new_node;
}

int forward_list_create(forward_list_t* list, size_t size_type)
{
	if (!list) return -1;

	list->begin = NULL;
	list->end = NULL;
	list->size = 0;
	list->size_type = size_type;
	list->comparator = NULL;

	return 0;
}


void forward_list_clear(forward_list_t* list)
{
	forward_node_t* next;
	forward_node_t* curr = list->begin;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	list->size = 0;
	list->begin = NULL;
	list->end = NULL;
}


void forward_list_destroy(forward_list_t* list)
{
	forward_list_clear(list);
	memset(list, 0x00, sizeof(forward_list_t));
}



void* forward_list_at(forward_list_t* list, int n)
{
	if (n >= list->size) return NULL;

	forward_node_t* node = _forward_list_node_at(list, n);

	return (void*)node->data;
}

void* forward_list_front(forward_list_t* list)
{
	if (list->begin) {
		return (void*)list->begin->data;
	}
	else {
		return NULL;
	}
}

void* forward_list_back(forward_list_t* list)
{
	if (list->end) {
		return (void*)list->end->data;
	}
	else {
		return NULL;
	}
}



int forward_list_assign(forward_list_t* list, int n, const void* data)
{
	if (n >= list->size) return -1;

	forward_node_t* node = _forward_list_node_at(list, n);

	memcpy(node->data, data, list->size_type);

	return 0;
}

forward_node_t* forward_list_push_front(forward_list_t* list, const void* data)
{
	forward_node_t* node = _forward_node_alloc(list);
	if (!node) return NULL; /* memory alloc error */

	memcpy(node->data, data, list->size_type);

	if (list->begin) {
		node->next = list->begin;
		list->begin = node;
	}
	else { /* first element in the list*/
		list->begin = node;
		list->end = node;
		node->next = NULL;
	}

	++list->size;

	return node;
}

forward_node_t* forward_list_push_back(forward_list_t* list, const void* data)
{
	forward_node_t* node = _forward_node_alloc(list);
	if (!node) return NULL; /* memory alloc error */

	memcpy(node->data, data, list->size_type);

	node->next = NULL; /* sent to the end so there is no next */

	if (list->end) {
		list->end->next = node;
		list->end = node;
	}
	else {
		list->begin = node;
		list->end = node;
	}

	++list->size;

	return node;

}

static forward_node_t* _forward_list_add_ordered_with(forward_list_t* list, const void* data, int (*comp)(const void*, const void*))
{

	if (!data) return NULL;

	if (list->size == 0) return forward_list_push_back(list, data);

	/* set node data */
	forward_node_t* new_node = _forward_node_alloc(list);
	if (!new_node) return NULL; /* memory alloc error */
	memcpy(new_node->data, data, list->size_type);

	/* init position to the list beginning and copy data to the new node*/
	forward_node_t* current = list->begin;
	forward_node_t* previous = NULL;

	/* find spot in list to add the new element */
	while (current != NULL && comp((&(current->data[0])), data) < 0) {
		previous = current;
		current = current->next;
	}

	new_node->next = current;

	if (previous && current == NULL) {
		/* iterated through the entire list: item will be added at the end */
		previous->next = new_node;
		list->end = new_node;
	}
	else if (previous == NULL) {
		/* hasn't even passed the first test: item will be added at the beginning */
		list->begin = new_node;
	}
	else {
		previous->next = new_node;
	}

	++list->size;

	return new_node;

}

forward_node_t* forward_list_add_ordered(forward_list_t* list, const void* data)
{
	if (list->comparator) {
		return _forward_list_add_ordered_with(list, data, list->comparator);
	}
	else {
		return NULL;
	}
}

forward_node_t* forward_list_insert_after(forward_list_t* list, forward_node_t* node, void* data)
{
	if (node == NULL) {
		return forward_list_push_front(list, data);
	}
	else if (node == list->end) {
		return forward_list_push_back(list, data);
	}
	else {
		forward_node_t* new_node = _forward_node_alloc(list);
		if (!new_node) return NULL; /* memory alloc error */
		memcpy(new_node->data, data, list->size_type);

		new_node->next = node->next;
		node->next = new_node;

		return new_node;
	}
}


int forward_list_set_comparator(forward_list_t* list, int (*comp)(const void*, const void*))
{
	list->comparator = comp;
	return 0;
}

forward_node_t* forward_list_contains(forward_list_t* list, const void* data)
{
	if (list->comparator) {

		forward_node_t* node = list->begin;

		while (node) {
			if (list->comparator(&(node->data[0]), data) == 0) {
				return node;
			}
			node = node->next;
		}

		return NULL;
	}
	else {
		return NULL;
	}
}





