#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h> 

#include <list>
#include <vector>
#include "list.h"
#include "vector.h"

#define BENCH_PUSH_BACK 10000000
#define BENCH_ITERATE   40000000
#define BENCH_SORT      1000000
#define RUN_COUNT       10

void escape(void* p)
{
    asm volatile("" : : "g"(p) : "memory");
}

void clobber()
{
    asm volatile("" : : : "memory");
}


int int_comparator(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}

double stdcontainers_list_push_back_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    list_t list;

    list_create(&list, sizeof(int));

    escape(&list);

    /* run RUN_COUNT times pushing BENCH_PUSH_BACK integers to a list */
    for(int j=0;j<RUN_COUNT;j++){
        list_clear(&list);
        start = clock();
        for(int i=0; i<BENCH_PUSH_BACK;i++){
            list_push_back(&list, &i);
        }
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }
    list_destroy(&list);
    
    time /= (double)RUN_COUNT;
    return time;
}

double stl_list_push_back_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    std::list<int> list;

    escape(&list);

    for(int j=0;j<RUN_COUNT;j++){
        list.clear();
        start = clock();
        for(int i=0; i<BENCH_PUSH_BACK;i++){
            list.push_back(i);
        }
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }
    list.clear();

    time /= (double)RUN_COUNT;
    return time;

}

double stdcontainers_list_iterate_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    list_t list;
    int value;

    list_create(&list, sizeof(int));

    escape(&list);
    escape(&value);

    /* populate list first */
    for(int i=0; i<BENCH_ITERATE;i++){
        list_push_back(&list, &i);
    }

    for(int j=0;j<RUN_COUNT;j++){
        start = clock();
        for(node_t* node = list.begin; node != NULL; node = node->next){
            value = *((int*)node->data);
        }
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }
    list_destroy(&list);

    time /= (double)RUN_COUNT;
    return time;

}

double stl_list_iterate_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    std::list<int> list;
    int value;

    escape(&list);
    escape(&value);

    /* populate list first */
    for(int i=0; i<BENCH_ITERATE;i++){
        list.push_back(i);
    }

    for(int j=0;j<RUN_COUNT;j++){
        start = clock();
        std::list<int>::iterator it;
        for (it = list.begin(); it != list.end(); ++it){
            value = *it;
        }
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }

    list.clear();

    time /= (double)RUN_COUNT;
    return time;
}



double stdcontainers_list_sort_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    list_t list;
    int value;

    list_create(&list, sizeof(int));
    list_set_comparator(&list, &int_comparator);

    escape(&list);
    escape(&value);

    for(int j=0;j<RUN_COUNT;j++){

        for(int i=0; i<BENCH_SORT;i++){
            value = rand();
            list_push_back(&list, &value);
        }

        start = clock();
        list_sort(&list);
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;

        list_clear(&list);
    }

    list_destroy(&list);

    time /= (double)RUN_COUNT;
    return time;
}


double stl_list_sort_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    std::list<int> list;
    int value;

    escape(&list);
    escape(&value);

    /* populate list first */

    for(int j=0;j<RUN_COUNT;j++){

        for(int i=0; i<BENCH_SORT;i++){
            value = rand();
            list.push_back(value);
        }

        start = clock();
        list.sort();
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;

        list.clear();
    }

    time /= (double)RUN_COUNT;
    return time;
}

double stdcontainers_vector_push_back_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    vector_t vector;

    vector_create(&vector, sizeof(int));

    escape(&vector);
    escape(&vector.data);

    for(int j=0;j<RUN_COUNT;j++){
        vector_clear(&vector);
        start = clock();
        for(int i=0; i<BENCH_PUSH_BACK;i++){
            vector_push_back(&vector, &i);
        }
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }
    vector_destroy(&vector);
    
    time /= (double)RUN_COUNT;

    return time;
}

double stl_vector_push_back_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    std::vector<int> vector;

    escape(&vector);


    for(int j=0;j<RUN_COUNT;j++){
        vector.clear();
        start = clock();
        for(int i=0; i<BENCH_PUSH_BACK;i++){
            vector.push_back(i);
        }
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }
    vector.clear();

    time /= (double)RUN_COUNT;
    return time;
}

double stdcontainers_vector_iterate_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    vector_t vector;
    int value;

    vector_create(&vector, sizeof(int));

    escape(&vector);
    escape(&vector.data);
    escape(&value);


    for(int i=0; i<BENCH_ITERATE;i++){
        vector_push_back(&vector, &i);
    }

    for(int j=0;j<RUN_COUNT;j++){

        start = clock();
        int* data = (int*)vector_front(&vector);
        for(int i=0; i< (int)vector.size; ++i){
            value = data[i];
        }
        end = clock();

        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }

    vector_destroy(&vector);
    time /= (double)RUN_COUNT;

    return time;
}

double stl_vector_iterate_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    std::vector<int> vector;
    int value;


    escape(&vector);
    escape(&value);


    /* populate list first */
    for(int i=0; i<BENCH_ITERATE;i++){
        vector.push_back(i);
    }

    for(int j=0;j<RUN_COUNT;j++){
        start = clock();
        std::vector<int>::iterator it;
        for (it = vector.begin(); it != vector.end(); ++it){
            value = *it;
        }
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;
    }

    vector.clear();

    time /= (double)RUN_COUNT;
    return time;
}

double stdcontainers_vector_sort_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    vector_t vector;
    int value;


    vector_create(&vector, sizeof(int));

    escape(&vector);
    escape(&vector.data);

    for(int j=0;j<RUN_COUNT;j++){

        for(int i=0; i<BENCH_SORT;i++){
            value = rand();
            vector_push_back(&vector, &value);
        }

        start = clock();
        vector_sort(&vector, &int_comparator);
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;

        vector_clear(&vector);
    }

    vector_destroy(&vector);

    time /= (double)RUN_COUNT;
    return time;
}


double stl_vector_sort_benchmark()
{
    clock_t start;
    clock_t end;
    double time = 0;
    std::vector<int> vector;
    int value;

    escape(&vector);

    for(int j=0;j<RUN_COUNT;j++){

        for(int i=0; i<BENCH_SORT;i++){
            value = rand();
            vector.push_back(value);
        }

        start = clock();
        sort(vector.begin(), vector.end());
        end = clock();
        time += ((double) (end - start)) / CLOCKS_PER_SEC;

        vector.clear();
    }

    time /= (double)RUN_COUNT;
    return time;
}


int main()
{
    srand(time(0));

    printf("stdcontainers C library vs C++ STL benchmark\n");
    printf("--------------------------------------------------\n");
    printf("-------------------|    list_t    |  std::list   |\n");
    printf("        push_back  | %11.4fs | %11.4fs | time to push %dM integers to a list\n", stdcontainers_list_push_back_benchmark(),  stl_list_push_back_benchmark(), BENCH_PUSH_BACK / 1000000);
    printf("          iterate  | %11.4fs | %11.4fs | time to iterate through %dM elements\n", stdcontainers_list_iterate_benchmark(), stl_list_iterate_benchmark(), BENCH_ITERATE / 1000000);
    printf("             sort  | %11.4fs | %11.4fs | time to sort %dM elements\n", stdcontainers_list_sort_benchmark(), stl_list_sort_benchmark(), BENCH_SORT / 1000000);
    printf("--------------------------------------------------\n");
    printf("-------------------|   vector_t   |  std::vector |\n");
    printf("        push_back  | %11.4fs | %11.4fs | time to push %dM integers to a vector\n", stdcontainers_vector_push_back_benchmark(),  stl_vector_push_back_benchmark(), BENCH_PUSH_BACK / 1000000);
    printf("          iterate  | %11.4fs | %11.4fs | time to iterate through %dM elements\n", stdcontainers_vector_iterate_benchmark(), stl_vector_iterate_benchmark(), BENCH_ITERATE / 1000000);
    printf("             sort  | %11.4fs | %11.4fs | time to sort %dM elements\n", stdcontainers_vector_sort_benchmark(), stl_vector_sort_benchmark(), BENCH_SORT / 1000000);
    printf("--------------------------------------------------\n");

    return 0;
}