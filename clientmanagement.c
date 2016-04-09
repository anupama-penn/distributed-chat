#include "clientmanagement.h"

void print_client_list() {
  int numclients = CLIENTS->numnodes;
    
  printf("Total # of Clients:\t%d\n", numclients);

  node_t* curr = CLIENTS->head;
  while(curr != NULL)
  {
    printf("%s %s: %d",((client_t*)curr->elem)->username,
	   ((client_t*)curr->elem)->hostname,
	   ((client_t*)curr->elem)->portnum);

    if(((client_t*)curr->elem)->isleader == TRUE)
            printf(" I...am...LEADER!!!");
    printf("\n");
    curr = curr->next;
  }
}


client_t* add_client(char username[], char hostname[], int portnum, bool_t isleader)
{
  client_t* newclient = (client_t*)malloc(sizeof(client_t));
  strcpy(newclient->username,username);
  strcpy(newclient->hostname,hostname);
  newclient->portnum = portnum;
  newclient->isleader = isleader;
  printf("About to add client\n");
  add_elem(CLIENTS,(void*)newclient);
  printf("Added client\n");
  return newclient;
}

void remove_client(char hostname[], int portnum)
{
  client_t* client;
  node_t* curr = CLIENTS->head;
  bool_t found = FALSE;
  while(curr != NULL)
  {
    client = ((client_t*)curr->elem);
    if(strcmp(hostname,client->hostname) == 0 && portnum == client->portnum)
    {
      found = TRUE;
      break;
    }
    curr = curr->next;
  }
  if(found)
  {
    remove_node(CLIENTS,curr);
  }
  free(client);
  client = NULL;
}
