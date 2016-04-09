#include "dchat.h"

void print_client_list();
client_t* add_client(char username[], char hostname[], int portnum, bool_t isleader);
void remove_client(char hostname[], int portnum);

