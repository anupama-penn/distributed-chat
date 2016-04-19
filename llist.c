
#include "llist.h"

void init_list(llist_t* list)
{  
  pthread_mutex_init(&list->mutex,NULL);
  pthread_mutex_lock(&list->mutex);
  list->head = NULL;
  list->tail = NULL;
  list->numnodes = 0;
  pthread_mutex_unlock(&list->mutex);
  return;
}


node_t* add_elem(llist_t* list, void* elem)
{
  node_t* node = (node_t*) malloc(sizeof(node_t));

  node->elem = elem;
  pthread_mutex_lock(&list->mutex);
  list->numnodes++;

  if(!list->head)
  {
    list->head = node;
    list->tail = list->head;
    list->tail->prev = NULL;
    list->tail->next = NULL;
    pthread_mutex_unlock(&list->mutex);
    return node;
  }

  list->tail->next = node;
  node->prev = list->tail;
  list->tail = node;
  list->tail->next = NULL;
  pthread_mutex_unlock(&list->mutex);
  return node;
}

void* remove_elem(struct llist_t* list, void* elem)
{
  node_t* curr = list->head;
  int index = 0;
  pthread_mutex_lock(&list->mutex);
  while(curr != NULL)
  {
    if(curr->elem == elem)
    {
      pthread_mutex_unlock(&list->mutex);
      return remove_node(list,curr);
    }
    index++;
    curr = curr->next;
  }
  pthread_mutex_unlock(&list->mutex);
  return NULL;
}

void* remove_node(llist_t* list, struct node_t* condemned)
{
  void* contents = condemned->elem;
  pthread_mutex_lock(&list->mutex);
  if(condemned->next)
    condemned->next->prev = condemned->prev;
  if(condemned->prev)
    condemned->prev->next = condemned->next;
  if(condemned == list->head)
    list->head = condemned->next;
  if(condemned == list->tail)
    list->tail = condemned->prev;
  free(condemned);
  list->numnodes--;
  pthread_mutex_unlock(&list->mutex);
  return contents;
}

void free_list(llist_t* condemned)
{
  node_t* curr = condemned->head;

  pthread_mutex_lock(&condemned->mutex);
  while(curr)
  {
    free(curr->elem);
    node_t* dying = curr;
    curr = curr->next;
    free(dying);
  }
  pthread_mutex_unlock(&condemned->mutex);
  free(condemned);
  return;
}
