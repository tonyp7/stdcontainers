#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "list.h"

int main()
{

    /* for random numbers */
    srand(time(0));

    /* declare the list */
    list_t list;

    /* create a list that will hold integers. */
    list_create(&list, sizeof(int));

    /* push 10 random integers to the list */
    int data, *value;
    for (int i = 0; i < 10; i++) {
        data = rand() % 100;
        list_push(&list, &data);
    }

    /* display these elements */
    printf("There are now %d elements in the list:\n", list.size);

    for(node_t* node = list.begin; node != NULL; node = node->next){
        value = (int*)node->data;
        printf("%2d ", *value);
    }
    printf("\n");

    /* release all memory occupied by the list's elements */
    list_destroy(&list);

    return 0;

}