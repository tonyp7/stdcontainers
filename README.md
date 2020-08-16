# stdcontainers
A collection of standard containers (lists, sorted lists, stacks, queues, vectors, trees) for C. The collection includes:
 
 - List
 - Sorted list
 - Stack
 - Queue
 - Double-ended queue (deque)
 - Vector
 - Binary tree

_stdcontainers_ follows naming conventions of the C++ stl containers when possible, making people feel instantly at home when using this library.

_stdcontainers_ is simple, straightforward C99 making it very friendly with low level programs and where portability is needed. 

# Content

 - [list.h](#listh)
   - [Basic example: a list of integers](#basic-example-a-list-of-integers)
   - [Iterating over a list](#iterating-over-a-list)
   - [Sorting a list](#sorting-a-list)
   - [List subtypes](#list-subtypes)
     - [Sorted List](#sorted-list)
     - [Queue](#queue)
     - [Double-ended queue (deque)](#double-ended-queue-deque)
 - [vector.h](#vectorh)

# list.h

list.h implements a generic doubly linked list, and is capable of holding any kind of data. The data is held at the node level without relying on void* pointers. list.h is the underlying implementation of many containers: sorted lists, queues and deques.

## Basic example: a list of integers

```c
/* create a list that will hold ints */
list_t* list = list_create(sizeof(int));

/* push some randoms integers to the list */
int data;
for (int i = 0; i < 10; i++) {
    data = rand() % 100;
    list_push(list, &data);
}

printf("There are now %d elements in the list\n", list->size);
list_clear(list);
printf("There are now %d elements in the list\n", list->size);
list_free(&list); /* list is now NULL */
```

## Iterating over a list

lists are meant to be iterated over using links instead of using indexes. While it is technically possible to iterate over a list using indexes, it is highly discouraged as it introduce a O(n^2) loop in your program.

## Storing complex types

Let's assume the following structure:

```c
typedef struct vector3f {
    float x;
    float y;
    float z;
}vector3f;
```

Storing this type of custom data in a list is just as simple as storing integers. Below is a simple example of how this can be achieved:

```c
list_t* list = list_create(sizeof(vector3f));
vector3f v, out;
v.x = 1.0; v.y = 2.0f; v.z = 3.0f;
list_push(list, &v);
list_peek(list, &out);
printf("Vector in list is x:%f y:%f z:%f\n", out.x, out.y, out.z);
````

## Sorting a list

list provides an efficident sorting method with a O(nlogn) complexity. To effectively sort a list, a comparator must be provided. Comparators are implemented with a int(void* a, void* b) signature. If the return value is negative, a is smaller than b. If the return value is 0, a equals b. If there return value is positive, a is bigger than b.

### Comparator for ints

A classic comparator for integers can be implemented as following:

```c
int int_comparator(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}
```

### Calling the sort function

Once a comparator is defined, it should be assigned to the list before calling list_sort.

```c
list_set_comparator(list, &int_comparator);
list_sort(list);
```

A list without comparator cannot perform a sorting operation.

If needed, it is possible to override the list's internal comparator by calling list_sort_with instead. For instance, given the comparator function below:

```c
int int_comparator_desc(const void* a, const void* b)
{
    return *((int*)b) - *((int*)a);
}
```

A list of integers could be sorted in descending order without changing its internal comparator with:

```c
list_sort_with(list, &int_comparator_desc);
```

## List subtypes

### Sorted List

### Queue

### Double-ended queue (deque)

# vector.h

vector.h implements a dynamic array, and is capable of holding any kind of data. Because of it's nature, a vector is extremely efficient at accessing random elements and adding/removing elements at its end. However, it fares very poorly when inserting/removing elements anywhere else.
vector.h is the underlying implementation of stack.
