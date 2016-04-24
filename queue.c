
//
// Implementation of a templatized queue
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"

void heapify(queue_t *q, int index);

int getParent(int index) {
  return index/2;
}

int getLeftChild(int index) {
  return 2*index + 1;
}

int getRightChild(int index) {
  return 2*index + 2;
}

queue_t *init(int (*cmp)(void *m1, void *m2), size_t init_capacity) {
    queue_t *q = NULL;
    q = malloc(sizeof(queue_t));
    pthread_mutex_init(&q->mutex,NULL);
    if(cmp == NULL || q == NULL) {
      return NULL;
    }
    q->cmp = cmp;
    q->messages = malloc(init_capacity * sizeof(*(q->messages)));
    if(q->messages == NULL) {
      return NULL;
    }
    q->num_elements = 0;
    q->capacity = init_capacity;
    return q;
}

void q_enqueue(queue_t *q, void *msg) {
  pthread_mutex_lock(&q->mutex);
    if(q == NULL) {
      pthread_mutex_unlock(&q->mutex);
      return;
    }
    if (q->num_elements >= q->capacity) {
        //REALLOC
      printf("Can't reallocate. Not implemented.\n");
    }
    q->messages[q->num_elements] = (void*) msg;
    int idx = q->num_elements;
    void *temp = NULL;
    while(idx > 0 && q->cmp(q->messages[idx], q->messages[getParent(idx)]) > 0) {
        temp = q->messages[idx];
        q->messages[idx] = q->messages[getParent(idx)];
        q->messages[getParent(idx)] = temp;
        idx = getParent(idx);
    }
    q->num_elements++;
    pthread_mutex_unlock(&q->mutex);
}

void *q_dequeue(queue_t *q) {
  pthread_mutex_lock(&q->mutex);
    if(q == NULL) {
      pthread_mutex_unlock(&q->mutex);
      return NULL;
    }
    if (q->num_elements < 1) {
      pthread_mutex_unlock(&q->mutex);
      return NULL;
    }
    void *item = q->messages[0];
    q->messages[0] = q->messages[q->num_elements-1];
    q->num_elements--;
    heapify(q, 0);
    pthread_mutex_unlock(&q->mutex);
    return (item);
}

void *q_peek(queue_t *q)
{
  pthread_mutex_lock(&q->mutex);
  if(q == NULL) {
    pthread_mutex_unlock(&q->mutex);
    return NULL;
  }
  if (q->num_elements < 1) {
    pthread_mutex_unlock(&q->mutex);
    return NULL;
  }
  void* returnval = q->messages[0];
  pthread_mutex_unlock(&q->mutex);
  return returnval;
}

void q_delete(queue_t *q) {
  pthread_mutex_lock(&q->mutex);
    if (q != NULL) {
      free(q->messages);
      free(q);
    }
  pthread_mutex_unlock(&q->mutex);
}

void heapify(queue_t *q, int index) {
    if(q == NULL) {
      return;
    }

    int max_index;
    int l_child = getLeftChild(index);
    int r_child = getRightChild(index);

    if (l_child < q->num_elements && q->cmp(q->messages[l_child], q->messages[index]) > 0) {
      max_index = l_child;
    } else {
      max_index = index;
    }

    if (r_child < q->num_elements && q->cmp(q->messages[r_child], q->messages[max_index]) > 0) {
      max_index = r_child;
    }

    if (max_index != index) {
      void *temp= q->messages[max_index];
      q->messages[max_index] = q->messages[index];
      q->messages[index] = temp;
      heapify(q, max_index);
    }
}
