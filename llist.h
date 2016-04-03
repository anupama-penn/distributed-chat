#include <stdlib.h>

typedef struct node_t //for use in llist_t
{
  void* elem;
  struct node_t* next;
  struct node_t* prev;
} node_t;

typedef struct llist_t //doubly-linked list implementation
{
  struct node_t* head;
  struct node_t* tail;
} llist_t;

//initializes empty list
void init_list(struct llist_t* list);

//adds new node containing elem to tail of list, 
struct node_t* add_elem(struct llist_t* list, void* elem);

//remove elem from list
void* remove_node(struct node_t* condemned);

//frees all the contents of list
void free_list(struct llist_t* condemned);
