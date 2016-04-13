#include <stddef.h>
#include <pthread.h>

typedef struct queue_t {
    int num_elements;
    int capacity;
    void **messages;
    int (*cmp)(void *m1, void *m2);
  pthread_mutex_t mutex;
} queue_t;

queue_t *init(int (*cmp)(void *m1, void *m2), size_t init_capacity);

void q_delete(queue_t *q);

void q_enqueue(queue_t *q, void *msg);

void *q_dequeue(queue_t *q);

void *q_peek(queue_t *q);
