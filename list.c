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

@file list.c
@author Tony Pottier
@brief Doubly linked list Standard C Container

@see https://github.com/tonyp7/stdcontainers

*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "list.h"

list_t* list_create(size_t data_size)
{

	list_t* list = (list_t*)malloc(sizeof(list_t));

	list->begin = NULL;
	list->end = NULL;
	list->size = 0;
	list->size_type = data_size;
	list->comparator = NULL;

	return list;
}


void list_free(list_t** list_ptr) 
{
	list_t* list = *list_ptr;
	node_t* next;
	node_t* curr = list->begin;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	free(list);
	*list_ptr = NULL;
}

void list_clear(list_t* list)
{
    node_t* next;
	node_t* curr = list->begin;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

    list->size = 0;
    list->begin = NULL;
    list->end = NULL;
}


int list_get(list_t* list, int n, void* data)
{
    if(n >= list->size) return -1;
    
    int i=0;
    node_t* curr = list->begin;
    while(i != n){
        curr = curr->next;
        i++;
    }

	if (data) {
		memcpy(data, curr->data, list->size_type);
	}
    
    return 0;
}

int list_set_comparator(list_t* list, int (*comp)(const void*, const void*)) 
{
	list->comparator = comp;
	return 0;
}


int list_push_back(list_t* list, const void* data) 
{
	node_t* node = (node_t*)malloc(sizeof(node_t) + sizeof(uint8_t) * list->size_type);
	if(!node) return -1; /* memory alloc error */
	
	memcpy( node->data , data, list->size_type);

	node->next = NULL; /* sent to the end so there is no next */
	if ( list->end != NULL) {
		node->previous = list->end;
		list->end->next = node;
		list->end = node;
	}
	else {
		list->begin = node;
		list->end = node;
		node->previous = NULL;
	}

	list->size++;
	
	return 0;
}






int list_pop_front(list_t* list, void* data) 
{
	if (list->begin) {
		node_t* first = list->begin; /* save ptr to free it later */
		
		/* NULL can be passed as data. In that case value isn't sent back to caller */
		if (data) {
			memcpy(data, list->begin->data, list->size_type);
		}
		
		if (list->end == list->begin) { /*edge case where list contains one element */
			list->begin = NULL;
			list->end = NULL;
		}
		else {
			list->begin = list->begin->next;
			list->begin->previous = NULL;
		}
		
		free(first);
		list->size--;
        
        return 0;
	}
	else {
		return -1;
	}
}



int list_pop_back(list_t* list, void* data)
{
	if (list->end) {

		node_t* last = list->end;

		if (data) {
			memcpy(data, list->end->data, list->size_type);
		}

		if (list->end == list->begin) { /*edge case where list contains one element */
			list->begin = NULL;
			list->end = NULL;
		}
		else {
			list->end = list->end->previous;
			list->end->next = NULL;
		}

		free(last);
		list->size--;

		return 0;

	}
	else {
		return -1;
	}
}


int list_peek_front(list_t* list, void* data)
{
    if (list->begin) {

		if (data) {
			memcpy(data, list->begin->data, list->size_type);
		}
        
        return 0;
    }
    else{
        return -1;
    }
}


int list_peek_back(list_t* list, void* data)
{
	if (list->end) {
		if (data) {
			memcpy(data, list->end->data, list->size_type);
		}

		return 0;
	}
	else {
		return -1;
	}
}



/**
 * @brief Split double linked lists in halves
 * At the end of the algorithm slow node will have done half the iteration as compared to fast
 * which will have reached the end of the list
 */
static node_t* split_list(node_t* head)
{
	node_t* fast = head, * slow = head;
	while (fast->next && fast->next->next)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	node_t* temp = slow->next;
	slow->next = NULL;
	return temp;
}

static node_t* merge_lists(node_t* first, node_t* second, int (*comp)(const void*, const void*))
{
	/* first list is empty ? */
	if (!first) {
		return second;
	}
		

	/* second list is empty ?*/
	if (!second) {
		return first;
	}
		
	/* run the comparison */
	if(   comp(&(first->data[0]), &(second->data[0])) < 0  )
	{
		first->next = merge_lists(first->next, second, comp);
		first->next->previous = first;
		first->previous = NULL;
		return first;
	}
	else
	{
		second->next = merge_lists(first, second->next, comp);
		second->next->previous = second;
		second->previous = NULL;
		return second;
	}
}

/**
 * @brief A classic recursive merge sort for double linked list
 * This algorithm will produce stack overflows for lists ~1500 items.
 * As such an iterative approach is preferred almost all the time
 */
static node_t* list_merge_sort_recursive(node_t* head, int (*comp)(const void*, const void*))
{
	if (!head || !head->next) {
		return head;
	}

	/* split list in halves */
	node_t* second = split_list(head);

	/* sort each sub list */
	head = list_merge_sort_recursive(head, comp);
	second = list_merge_sort_recursive(second, comp);

	/* merge them back together */
	return merge_lists(head, second, comp);
}




/**
 * @brief Used by list_merge_sort_bottom_up to merge two sorted lists
 * @see list_merge_sort_bottom_up
 */
static node_t* list_merge_lists(node_t* list1, node_t* list2, int (*comp)(const void*, const void*))
{
	node_t* head = NULL;
	node_t** p_head = &head;

	if (list1 == NULL) {
		return list2;
	}
		
	if (list2 == NULL) {
		return list1;
	}
		
	for(;;) {
		if (comp(&(list2->data[0]), &(list1->data[0])) < 0) {
			*p_head = list2;
			list2 = *(p_head = &(list2->next));
			if (list2 == NULL) {
				*p_head = list1;
				break;
			}
		}
		else {
			*p_head = list1;
			list1 = *(p_head = &(list1->next));
			if (list1 == NULL) {
				*p_head = list2;
				break;
			}
		}
	}
	return head;
}



#define MERGE_SORT_BOTTOM_UP_NUMLISTS 32

/**
 *
 * @see https://en.wikipedia.org/wiki/Merge_sort#Bottom-up_implementation_using_lists
 */
static int list_merge_sort_bottom_up(list_t* list, int (*comp)(const void*, const void*))
{
	node_t* lists[MERGE_SORT_BOTTOM_UP_NUMLISTS];
	node_t* node;
	node_t* next;
	int i;

	/* list is empty? there's nothing to do */
	if (list->begin == NULL) {
		return 0;
	}
		
	/* Set all pointers to node as NULL */
	memset(lists, 0, sizeof(lists));

	node = list->begin;
	while (node != NULL) {
		next = node->next;
		node->next = NULL;
		for (i = 0; (i < MERGE_SORT_BOTTOM_UP_NUMLISTS) && (lists[i] != NULL); i++) {
			node = list_merge_lists(lists[i], node, comp);
			lists[i] = NULL;
		}
		if (i == MERGE_SORT_BOTTOM_UP_NUMLISTS) {
			i--;
		}
			
		lists[i] = node;
		node = next;
	}
	node = NULL;

	for (i = 0; i < MERGE_SORT_BOTTOM_UP_NUMLISTS; i++) {
		node = list_merge_lists(lists[i], node, comp);
	}
		
	/* node now contains the final head of the sorted list so it's saved as such */
	list->begin = node;
	
	/* restore previous links and list's last element */
	list->begin->previous = NULL;
	while (node->next != NULL) {
		node->next->previous = node;
		node = node->next;
	}
	list->end = node;

	return 0;
}



int list_sort(list_t* list)
{
	if (list->comparator) {
		return list_merge_sort_bottom_up(list, list->comparator);
	}
	else {
		return -1;
	}
	
}


int list_sort_with(list_t* list, int(*comp)(const void*, const void*)) 
{
	return list_merge_sort_bottom_up(list, comp);
}

bool list_contains(list_t* list, const void* data)
{
	if (list->comparator) {
		node_t* node = list->begin;

		while (node) {
			if (list->comparator(&(node->data[0]), data) == 0) {
				return true;
			}
			node = node->next;
		}

		return false;
	}
	else {
		return false;
	}
}


int list_add_after(list_t* list, node_t* node, void* data) 
{
	if (!data) return -1;

	node_t* new_node = (node_t*)malloc(sizeof(node_t) + sizeof(uint8_t) * list->size_type);
	if (!new_node) return -1; /* memory alloc error */

	memcpy(new_node->data, data, list->size_type);

	new_node->previous = node;

	if (node == NULL) {
		/* insert at beginning of list*/
		new_node->next = list->begin;
		list->begin->previous = new_node;
		list->begin = new_node;
	}
	else if (node->next == NULL) {
		/* insert at end of list*/
		new_node->next = NULL;
		node->next = new_node;
		list->end = new_node;
	}
	else {
		new_node->next = node->next;
		node->next->previous = new_node;
		node->next = new_node;
	}
	
	list->size++;

	return 0;

}

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 6011) /* C6011: dereferencing NULL pointer <name> */ 
#endif
static int list_add_ordered_with(list_t* list, const void* data, int (*comp)(const void*, const void*))
{

	if (!data) return -1;

	if (list->size == 0) return list_push_back(list, data);

	/* set node data */
	node_t* new_node = (node_t*)malloc(sizeof(node_t) + sizeof(uint8_t) * list->size_type);
	if (!new_node) return -1; /* memory alloc error */
	memcpy(new_node->data, data, list->size_type);
	
	/* init position to the list beginning and copy data to the new node*/
	node_t* current = list->begin;
	node_t* previous = NULL;

	/* find spot in list to add the new element */
	while (current != NULL && comp( (&(current->data[0])), data) < 0) {
		previous = current;
		current = current->next;
	}

	new_node->previous = previous;
	new_node->next = current;

	if (current == NULL) {
		/* iterated through the entire list: item will be added at the end */
		previous->next = new_node;
		list->end = new_node;
	}
	if (previous == NULL) {
		/* hasn't even passed the first test: item will be added at the beginning */
		current->previous = new_node;
		list->begin = new_node;
	}
	else {
		previous->next->previous = new_node;
		previous->next = new_node;
	}

	list->size++;

	return 0;
}
#if defined(_MSC_VER)
#pragma warning(pop)
#endif


int list_add_ordered(list_t* list, const void* data) {

	if (list->comparator) {
		return list_add_ordered_with(list, data, list->comparator);
	}
	else {
		return -1;
	}

}