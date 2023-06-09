typedef struct priority_queue_t{
    void** data;
    int* priorities;

    unsigned int size;
    unsigned int capacity;
} Priority_queue;


Priority_queue* priority_queue_init();
unsigned int priority_queue_size(Priority_queue* vec);
void priority_queue_push(Priority_queue* vec, void* value, int priority);
void* priority_queue_pop(Priority_queue* vec);
int priority_queue_contains(Priority_queue* vec, void* value);
void* priority_queue_at(Priority_queue* vec, unsigned int index);
void priority_queue_clear(Priority_queue* vec);
void priority_queue_destroy(Priority_queue* vec);
void priority_queue_print(Priority_queue* vec);
