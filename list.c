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

	list->first = NULL;
	list->last = NULL;
	list->size = 0;
	list->data_size = data_size;

	return list;
}


void list_free(list_t* list) 
{

	node_t* next;
	node_t* curr = list->first;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	free(list);
}

void list_clear(list_t* list)
{
    node_t* next;
	node_t* curr = list->first;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

    list->size = 0;
    list->first = NULL;
    list->last = NULL;
}


int list_get(list_t* list, int n, void* data)
{
    if(n >= list->size) return -1;
    
    int i=0;
    node_t* curr = list->first;
    while(i != n){
        curr = curr->next;
        i++;
    }

	if (data) {
		memcpy(data, curr->data, list->data_size);
	}
    
    return 0;
}


int list_push_back(list_t* list, void* data) 
{
	node_t* node = (node_t*)malloc(sizeof(node_t) + sizeof(uint8_t) * list->data_size);
	if(!node) return -1; /* memory alloc error */
	
	memcpy( node->data , data, list->data_size);

	node->next = NULL; /* sent to the end so there is no next */
	if ( list->last != NULL) {
		node->previous = list->last;
		list->last->next = node;
		list->last = node;
	}
	else {
		list->first = node;
		list->last = node;
		node->previous = NULL;
	}

	list->size++;
	
	return 0;
}



int list_add_ordered(list_t* list, void* data, int (*comp)(void*, void*))
{

	if (list->size == 0) return list_push_back(list, data);
	if (!data) return -1;

    node_t* node = (node_t*)malloc(sizeof(node_t) + sizeof(uint8_t) * list->data_size);
    if(!node) return -1; /* memory alloc error */
    
    /* init position to the list beginning and copy data to the new node*/
	node_t* current = list->first;
	node_t* previous = NULL;
	memcpy( node->data , data, list->data_size);
	

	/* find spot in list to add the new element */
	while (current != NULL && comp( &(node->data[0]), &data) < 0) {
		previous = current;
		current = current->next;
	}
	node->next = current;

	if (previous == NULL) {
		/* added at the beginning of the list */
		list->first = node;

		/* in case this was the first element to be added */
		if (list->last == NULL) {
			list->last = node;
		}
	}
	else {

		/* the end of the list is the newly inserted node ? */
		if (list->last == previous) {
			list->last = node;
		}
		previous->next = node;
	}


	list->size++;
	
	return 0;

}


int list_pop_front(list_t* list, void* data) 
{
	if (list->first) {
		node_t* first = list->first; /* save ptr to free it later */
		
		/* NULL can be passed as data. In that case value isn't sent back to caller */
		if (data) {
			memcpy(data, list->first->data, list->data_size);
		}
		
		if (list->last == list->first) { /*edge case where list contains one element */
			list->first = NULL;
			list->last = NULL;
		}
		else {
			list->first = list->first->next;
			list->first->previous = NULL;
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
	if (list->last) {

		node_t* last = list->last;

		if (data) {
			memcpy(data, list->last->data, list->data_size);
		}

		if (list->last == list->first) { /*edge case where list contains one element */
			list->first = NULL;
			list->last = NULL;
		}
		else {
			list->last = list->last->previous;
			list->last->next = NULL;
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
    if (list->first) {

		if (data) {
			memcpy(data, list->first->data, list->data_size);
		}
        
        return 0;
    }
    else{
        return -1;
    }
}


int list_peek_back(list_t* list, void* data)
{
	if (list->last) {
		if (data) {
			memcpy(data, list->last->data, list->data_size);
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

static node_t* merge_lists(node_t* first, node_t* second, int (*comp)(void*, void*))
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
static node_t* list_merge_sort_recursive(node_t* head, int (*comp)(void*, void*))
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
static node_t* list_merge_lists(node_t* list1, node_t* list2, int (*comp)(void*, void*))
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
static int list_merge_sort_bottom_up(list_t* list, int (*comp)(void*, void*))
{
	node_t* lists[MERGE_SORT_BOTTOM_UP_NUMLISTS];
	node_t* node;
	node_t* next;
	int i;

	/* list is empty? there's nothing to do */
	if (list->first == NULL) {
		return;
	}
		
	/* Set all pointers to node as NULL */
	memset(lists, 0, sizeof(lists));

	node = list->first;
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
	list->first = node;
	
	/* restore previous links and list's last element */
	list->first->previous = NULL;
	while (node->next != NULL) {
		node->next->previous = node;
		node = node->next;
	}
	list->last = node;

	return 0;
}



int list_sort(list_t* list, int (*comp)(void*, void*))
{
	return list_merge_sort_bottom_up(list, comp);
}