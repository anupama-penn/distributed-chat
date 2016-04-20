#include "dchat.h"

pthread_mutex_t client_deference_mutex;

void print_client_list();
client_t* create_client(char username[], char hostname[], int portnum, bool isleader);
client_t* add_client(char username[], char hostname[], int portnum, bool isleader);
void remove_client(char hostname[], int portnum);
void remove_client_by_uid(char uid[]);
client_t* find_first_client_by_username(char username[]);
client_t* find_client_by_uid(char uid[]);
client_t* find_curr_leader();
void clear_deference();