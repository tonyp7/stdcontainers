# stdcontainers
A collection of standard containers (lists, sorted lists, stacks, queues, deques, vectors) for C.
 
_stdcontainers_ follows naming conventions of the C++ stl containers when possible, making people feel instantly at home when using this library.

_stdcontainers_ is simple, straightforward C99 making it very friendly with low level programs and where portability is needed. 

# Content

 - [list.h](#listh)
   - [Basic example: a list of integers](#basic-example-a-list-of-integers)
   - [Iterating over a list](#iterating-over-a-list)
   - [Sorting a list](#sorting-a-list)
   - [List subtypes: sorted list, queue, double-ended queue (deque)](#list-subtypes-sorted-list-queue-double-ended-queue-deque)
 - [vector.h](#vectorh)
 - [btree.h](#btreeh)
 - [Benchmarks](#benchmarks)

# list.h

list.h implements a generic doubly linked list, and is capable of holding any kind of data. The data is held at the node level without relying on void* pointers. list.h is the underlying implementation of many containers: sorted lists, queues and deques.

## Basic example: a list of integers

```c
/* create a list that will hold ints */
list_t list;
list_create(&list, sizeof(int));

/* push some randoms integers to the list */
int data;
for (int i = 0; i < 10; i++) {
    data = rand() % 100;
    list_push(&list, &data);
}

printf("There are now %d elements in the list\n", list->size);
list_clear(&list);
printf("There are now %d elements in the list\n", list->size);
```

## Iterating over a list

lists are meant to be iterated over using links instead of using indexes. While it is technically possible to iterate over a list using indexes, it is highly discouraged as it introduce a O(n^2) loop --n(n+1) / 2 operations to be precise-- in your program.

```c
/* DO NOT DO THIS */
for(int i=0, *value; i<list.size; i++){
    value = list_at(&list, i);
    printf("%d ", *value);
}
```

iterating through a list should be done by going through each individual node. As in:

```c
/* this an example of an extremely efficient list iteration */
int *value;
for(node_t* node = list.begin; node != NULL; node = node->next){
    value = (int*)node->data;
    printf("%d ", *value);
}
```

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
list_t list;
vector3f v, out;
list_create(&list, sizeof(vector3f));
v.x = 1.0; v.y = 2.0f; v.z = 3.0f;
list_push(&list, &v);
out = *(vector3f*)list_peek(&list);
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
list_set_comparator(&list, &int_comparator);
list_sort(&list);
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
list_sort_with(&list, &int_comparator_desc);
```

## List subtypes: Sorted List, Queue, Double-ended queue (deque)

You can restrict the general implementation of list.h by using specialized containers. For instance, it is impossible to add an element to the front of a queue. A queue is a first in first out structure where the elements are always added at the back.

Below is a table summarizing what can and can't be done with the specialized containers:

| Action  | Description | Sorted List | Queue | Deque | Note |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| at | access a ramdom element | :heavy_check_mark: | :x: | :x: | |
| front | access the first element | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | |
| back | access the last element | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | |
| insert | insert at random location | :x: | :x: | :x: | |
| push_front | insert at beginning | :x: | :x: | :heavy_check_mark: | |
| push_back | insert at end | :x: | :heavy_check_mark: | :heavy_check_mark: | queue's push_back is simply known as push |
| add_ordered | insert in order | :heavy_check_mark: | :x: | :x: | sorted's list only allowed operation to add an item |
| erase | remove a random element | :heavy_check_mark: | :x: | :x: | |
| pop_front | remove the first element | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | queue's pop_front is simply known as pop |
| pop_back | remove the last element | :heavy_check_mark: | :x: | :heavy_check_mark: | |
| sort | sort the list | :x: | :x: | :x: | a sorted list is naturally sorted |

# vector.h

vector.h implements a dynamic array, and is capable of holding any kind of data. Because of it's nature, a vector is extremely efficient at accessing random elements and adding/removing elements at its end. However, it fares very poorly when inserting/removing elements anywhere else.
vector.h is the underlying implementation of stack.

## Basic example: a vector of integers


## Iterating over a vector

A vector is a dynamic array. At its heart, it is still an array. This means it is entirely possible to iterate over a vector like you would do with a normal array. The example below demonstrates this:

```c
/* IMPORTANT: see vector caveat */
int* data = (int*)vector_front(&vector);
for(int i=0; i<vector.size; i++){
    printf("%d ", data[i]);
}
```

### Vector caveat

As the vector dynamically grows and shrinks, its internal data pointer may change location as the underlying memory allocation implementation may see fit. As a result, you should not rely on a local pointer to the internal array as demonstrated above. A safer iteration as demonstrated above can be done as following:

```c
int* data;
for(int i=0; i<vector.size; i++){
    data = (int*)vector_at(&vector, i);
    printf("%d ", *data);
}
```

If your code is mono-threaded and no insertion/deletion are done while iterating, both methods are valid. Please do note that no 



