#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"


//* private functions --------------------
void resize(Priority_queue* q, unsigned int new_capacity) {
    q->capacity = new_capacity;
    q->data = (void**) realloc(q->data, new_capacity * sizeof(void*));
    q->priorities = (int*) realloc(q->priorities, new_capacity * sizeof(int));
};

void sort_priorities(Priority_queue* q) {
    for(int i = 0; i < q->size - 1; i++) {
        for(int j = i + 1; j < q->size; j++) {
            if(q->priorities[i] > q->priorities[j]) {
                int tmp = q->priorities[i];
                q->priorities[i] = q->priorities[j]; 
                q->priorities[j] = tmp; 

                void* temp = q->data[i];
                q->data[i] = q->data[j];
                q->data[j] = temp;
            }
        }
    }
};
//* --------------------------------------

Priority_queue* priority_queue_init() {
    Priority_queue* q = (Priority_queue*) malloc(sizeof(Priority_queue));

    q->data = NULL;
    q->size = 0;
    q->capacity = 0;

    return q;
};


unsigned int priority_queue_size(Priority_queue* q) {
    return q->size;
};


void priority_queue_push(Priority_queue* q, void* value, int priority) {
    if(q->size >= q->capacity) {
        if(q->capacity == 0) resize(q, 2);
        else resize(q, q->capacity * 2);
    }
    
    q->priorities[q->size] = priority;
    q->data[q->size++] = value;

    sort_priorities(q);
};


void* priority_queue_pop(Priority_queue* q) {
    if(q->size == 0) {
        printf("ERROR! No elements to pop!");
        return NULL;
    }

    void* returned_value = q->data[0];

    for(int i = 0; i < q->size - 1; q->data[i] = q->data[i + 1], i++);

    (q->size)--;

    return returned_value;
};


int priority_queue_contains(Priority_queue* q, void* value) {
    for(int i = 0; i < q->size; i++) {
        if(q->data[i] == value) return 1;
    }

    return 0;
};


void* priority_queue_at(Priority_queue* q, unsigned int index) {
    if(index > q->size) {
        printf("ERROR! Index out of reach!");
        return NULL;
    }

    return q->data[index];
};


void priority_queue_clear(Priority_queue* q) {
    free(q->data);
    q->size = 0;
    q->capacity = 0;
};


void priority_queue_destroy(Priority_queue* q) {
    free(q->data);
    q->size = 0;
    q->capacity = 0;

    free(q);
};


void priority_queue_print(Priority_queue* q) {
    for(int i = 0; i < q->size; i++)
    	printf("Priority_queue[%d] = %d\n", i, *((int*)priority_queue_at(q, i)));
           //! SET UP YOUR DATA TYPE HERE ^^^
                                   //!    ||
};


