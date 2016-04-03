
typedef struct Q {
    int32_t num_elements;
    int32_t capacity;
    void **messages;
    int (*cmp)(const void *m1, const void *m2);
} Queue;

Queue *init(int (*cmp)(const void *m1, const void *m2), size_t init_capacity);

void q_delete(Queue *q);

void q_enqueue(Queue *q, const void *msg);

void *q_dequeue(Queue *q);
