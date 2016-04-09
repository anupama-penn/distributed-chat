#include <stddef.h>

typedef struct queue_t {
    int num_elements;
    int capacity;
    void **messages;
    int (*cmp)(const void *m1, const void *m2);
} queue_t;

queue_t *init(int (*cmp)(void *m1, void *m2), size_t init_capacity);

void q_delete(queue_t *q);

void q_enqueue(queue_t *q, const void *msg);

void *q_dequeue(queue_t *q);
