# stdcontainers
A collection of standard containers (lists, sorted lists, stacks, queues, deques, vectors) for C.
 
_stdcontainers_ follows naming conventions of the C++ stl containers when possible, making people feel instantly at home when using this library.

_stdcontainers_ is simple, straightforward C99 making it very friendly with low level programs and where portability is needed.

If you are working in a pure C environment, or you do not have access to the C++ STL for any reason, _stdcontainers_ allows you to work with high level containers with zero overhead cost.

# Content

 - [list.h](#listh)
   - [Basic example: a list of integers](#basic-example-a-list-of-integers)
   - [Iterating over a list](#iterating-over-a-list)
   - [Sorting a list](#sorting-a-list)
   - [List subtypes: sorted list, queue, double-ended queue (deque)](#list-subtypes-sorted-list-queue-double-ended-queue-deque)
 - [vector.h](#vectorh)
   - [Basic example: a vector of integers](#basic-example-a-vector-of-integers)
   - [Iterating over a vector](#iterating-over-a-vector)
   - [Sorting a vector](#sorting-a-vector)
 - [forward_list.h](#forward_listh)
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

printf("There are now %d elements in the list\n", list.size);
list_clear(&list);
printf("There are now %d elements in the list\n", list.size);
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

C++ hides list nodes behind the iteration abstraction. While it would be technically possible to implement something similar in C and it has been done multiple times, the added value is debatable. As a result, this library voluntarily chooses be close to the metal. 

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

vector.h implements a dynamic array, and is capable of holding any kind of data. Because of its nature, a vector is extremely efficient at accessing random elements and adding/removing elements at its end. However, it fares very poorly when inserting/removing elements anywhere else.
vector.h is the underlying implementation of stack_t.

## Basic example: a vector of integers

```c
vector_t vector;
vector_create(&vector, sizeof(int));

/* push 10 integers to the vector */
for(int i=0; i < 10; i++){
    vector_push_back(&vector, &i);
}

/* clear the vector without detroying it */
vector_clear(&vector);

/* destroy the vector: any operation on the vector is now invalid */
vector_destroy(&vector);
```

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

If your code is mono-threaded and no insertion/deletion are done while iterating, both methods are valid. There is no performance advantage/didsadvantage to doing one or the other so it is recommended to use vector_at at all time.

### Sorting a vector

Sorting a vector works exactly the same as sorting a list. A standard comparator must be define, similarly to a list. e.g:

```c
int int_comparator(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}
```

Then sorting a vector is as simple as calling vector_sort.

```c
vector_sort(&vector, &int_comparator);
```

Here's a complete example pushing 10 random integers to a vector and sorting them:

```c
vector_t vector;
vector_create(&vector, sizeof(int));
for(int i=0; i < 10; i++){
    int data = rand() % 100;
    printf("%d ", data);
    vector_push_back(&vector, &data);
}

printf("\n");

vector_sort(&vector, &int_comparator);
int* value;
for(int i=0; i < vector.size; i++){
    value = (int*)vector_at(&vector, i);
    printf("%d ", *value);
}
```

This should create an output similar to this:

```bash
83 86 77 15 93 35 86 92 49 21
15 21 35 49 77 83 86 86 92 93
```

Internally, a vector is sorted using the quick sort algorithm.

# forward_list.h

forward_list.h implements a singly linked list, similarly to STL's include <forward_list>. Forward lists cannot be iterated backwards, but they have the advantage of being slightly more lightweight as compared to their traditional list counterpart. The overhead on each node is half that of a doubly linked list, dropping the pointer to the previous node (64 or 32 bit depending on the architecture).

As a result, embedded systems with limited ram should consider forward_list.h instead of list.h. The loss of versatility is often not worth it on a modern PC.

# Benchmarks

Due to the very low level of its implementation, _stdcontainers_ is fast. There's a benchmark subfolder you can check out where stdcontainers is pitted against the C++ STL.

Results shown below are on a core i7 7700k, 16GB of RAM, Ubuntu 20.04. Code is compiled with -Wall -O2 flags:

|  type: int |    list_t    |  std::list   | note |
| ---------- | ------------ | ------------ | ---- |
| push_back  |      0.1164s |      0.1177s | time to push 10M integers to a list |
|  iterate  |      0.1107s |      0.0912s | time to iterate through 40M elements |
| sort  |      0.4294s |      0.4008s | time to sort 1M elements |


|  type: int | vector_t     |  std::vector | note |
| ---------- | ------------ | ------------ | ---- |
| push_back  |      0.0360s |      0.0415s | time to push 10M integers to a vector |
| iterate  |      0.0136s |      0.0136s | time to iterate through 40M elements |
| sort  |      0.0902s |      0.0547s | time to sort 1M elements |

True to its core concepts of "0 cost overhead" and "you wouldn't be able to handcode it faster", the STL is blazingly fast in many scenarios but _stdcontainers_ is within margin of error for both iterations and push_back. Sorting seems to be consistently faster on the STL. The STL std::sort is notoriously optimized at compile time for basic types (such as int) which gives an edge on a vector sort.

If we define a custom type such a two components vector and their respective C and C++ comparators (e.g. their squared Euclidian norm), then all all of a sudden not only the STL loses its edge, it is also actually slower:

```c
typedef struct vector2f{
    float x;
    float y;
}vector2f;

/* C style comparator for a vector2f */
int vector2f_comparator(const void* a, const void* b)
{
    vector2f* va = (vector2f*)a;
    vector2f* vb = (vector2f*)b;
    float norm_va = ((va->x * va->x ) + (va->y * va->y));
    float norm_vb = ((vb->x * vb->x ) + (vb->y * vb->y));

    return  (norm_va > norm_vb) - (norm_va < norm_vb);
}

/* C++ style comparator for a vector 2f */
struct stl_vector2f_comparator
{
    bool operator ()(const vector2f & va, const vector2f & vb)
    {
        return (va.x * va.x + va.y * va.y) < ( vb.x * vb.x + vb.y * vb.y );
    }
};
```

|   type: vector2f  |   vector_t   | std::vector  | note                          |
| ----------------- | ------------ | ------------ | ----------------------------- |
|             sort  |      0.0679s |      0.0796s | time to sort 1M elements      |


These numbers demonstrate that _stdcontainers_ is competitive when it comes to speed.
