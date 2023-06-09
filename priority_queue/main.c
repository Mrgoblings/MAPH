#include <stdio.h>
#include "priority_queue.h"


int main() {
    int input[] = {13, 7, 15, 120, 25};

    Priority_queue* q = priority_queue_init();
 
    for(int i = 0; i < 5; i ++)
    	priority_queue_push(q, input + i, input[i]);
   
    printf("Size is %d\n", priority_queue_size(q));

    priority_queue_print(q);
    
    printf("pop = %d\n", *((int*)priority_queue_pop(q)));

    priority_queue_print(q);

    priority_queue_destroy(q);
    q = NULL;
 
    return 0;
}