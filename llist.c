
#include "llist.h"

void init_list(llist_t* list)
{  
  list->head = NULL;
  list->tail = NULL;
  list->numnodes = 0;
  return;
}


node_t* add_elem(llist_t* list, void* elem)
{
  node_t* node = (node_t*) malloc(sizeof(node_t));

  node->elem = elem;
  list->numnodes++;

  if(!list->head)
  {
    list->head = node;
    list->tail = list->head;
    list->tail->prev = NULL;
    list->tail->next = NULL;
    return node;
  }

  list->tail->next = node;
  node->prev = list->tail;
  list->tail = node;
  list->tail->next = NULL;
  return node;
}

void* remove_node(llist_t* list, struct node_t* condemned)
{
  void* contents = condemned->elem;
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
  return contents;
}

void free_list(llist_t* condemned)
{
  node_t* curr = condemned->head;

  while(curr)
  {
    free(curr->elem);
    node_t* dying = curr;
    curr = curr->next;
    free(dying);
  }
  return;
}
