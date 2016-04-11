#include "dchat.h"
#include "messagingprotocol.h"
#include "clientmanagement.h"
#include "messagemanagement.h"

void error(char *x){
  perror(x);
  exit(1);
}

// add some way to check if client is alive

bool initialize_data_structures() {
    
  init_list(CLIENTS);
  init_list(UNSEQ_CHAT_MSGS);
  HBACK_Q = init(message_compare,100);

  //  INITIALIZED = TRUE;
  //  return INITIALIZED;
  return TRUE;
}

/*
//TODO a long time from now or probably never
void destroy_data_structures() {

    if (initialized == TRUE) {
        if (clients != NULL) {
            if (clients -> clientlist.clientlist_val != NULL) {
                free(clients->clientlist.clientlist_val);
            }
            free(clients);
        }
        if (msg_buffer != NULL) {
            if (msg_buffer->msg_send !=NULL) {
                free(msg_buffer->msg_send);
            }
            if (msg_buffer->user_sent !=NULL) {
                free(msg_buffer->user_sent);
            }
            //no need to free seq_num and MSG type
        }
    }
    }*/

/*
void shutdown(){
n    
    destroy_data_structures();
    exit(0);
    return NULL;
}

*/

void send_chat_message(int counter, char userinput[])
{
   char* sendstr = strdup(userinput);
   int timestamp = (int)time(NULL);
   char uid[MAXUIDLEN];
   sprintf(uid,"%d^_^%d",timestamp,counter);
   multicast_UDP(CHAT,me->username, uid, sendstr);
   free(sendstr);
}

void *get_user_input(void* t)
{
  char userinput[MAXCHATMESSAGELEN];
  int counter = 0;
  int i = 0;

  if(UIRUNNING)
    initui(0);
  else
  {
    while(1)
    {
      fgets(userinput, sizeof(userinput), stdin);
      if(userinput[0] == '\n')
	continue;
      for(i = 0; i < strlen(userinput); i++)
      {
	if(userinput[i]=='\n')
	  userinput[i] = ' ';
      }
      //    scanf("%s",userinput);
      //    usleep(10000);
      counter++;
      send_chat_message(counter, userinput);
    }
  }
  pthread_exit((void *)t);
}

void *checkup_on_clients(void* t)
{

  int counter = 0;
  while(1)
  {
    sleep(1); // Interval between checkups
    
    int timestamp = (int)time(NULL);
    char uid[MAXUIDLEN];
    sprintf(uid,"%d^_^%d",timestamp,counter);
    
    multicast_UDP(CHECKUP,me->username, uid, "ARE_YOU_ALIVE"); // multicast checkup message to everyone

    node_t* curr = CLIENTS->head;
    while(curr != NULL)
    {
      // increment everyones counter by one until they respond
      ((client_t*)curr->elem)->missed_checkups++;

      /*
      * Just for debugging purposes to see the current status of missed_checkups for each client
      
      printf("%s %s: %d %d\n",((client_t*)curr->elem)->username,
     ((client_t*)curr->elem)->hostname,
     ((client_t*)curr->elem)->portnum,
     ((client_t*)curr->elem)->missed_checkups);
     */

      // check if anyone has missed too many checkups
      if (((client_t*)curr->elem)->missed_checkups > 4)
      {
        printf("I (%s) believe that (%s) is dead :)\n", me->username, ((client_t*)curr->elem)->username);
      }

      curr = curr->next;
    }
    counter++;
  }
  pthread_exit((void *)t);
}

void create_message_threads()
{
  int numthreads = 3;
  pthread_t threads[numthreads];
  pthread_attr_t attr;
  void *exitstatus;
  
  //make sure threads are joinable
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  //start a thread for each
  pthread_create(&threads[RECEIVE_THREADNUM], &attr, receive_UDP, (void *)RECEIVE_THREADNUM);
  pthread_create(&threads[SEND_THREADNUM], &attr, get_user_input, (void *)SEND_THREADNUM);
  pthread_create(&threads[CHECKUP_THREADNUM], &attr, checkup_on_clients, (void *)CHECKUP_THREADNUM);

  pthread_join(threads[RECEIVE_THREADNUM], &exitstatus);
  pthread_join(threads[SEND_THREADNUM], &exitstatus);
  pthread_join(threads[CHECKUP_THREADNUM], &exitstatus);
}

int main(int argc, char* argv[]){
    
  UNSEQ_CHAT_MSGS = (llist_t*) malloc(sizeof(llist_t));
  CLIENTS = (llist_t*) malloc(sizeof(llist_t));
  initialize_data_structures();

  printf("I'm awake.\n");
  UIRUNNING = 0;

  if(strcmp(argv[1],"5000"))
  {
    LOCALPORT = 6000;
  }
  else if(strcmp(argv[1],"6000"))
  {
    LOCALPORT = 5000;    
  }
  LOCALHOSTNAME = "127.0.0.1";
  printf("I'm %d\n",LOCALPORT);
  //add the other fake guy
  add_client("leader\0","127.0.0.1\0",5000,TRUE);
  add_client("follower\0","127.0.0.1\0",6000,FALSE);

  if(me->isleader)
    printf("***I AM LEADER!!!***\n");

  SEQ_NO = 0;
  LEADER_SEQ_NO = 0;
  create_message_threads();

  return 0;
  /*    pid_t pID = fork();
    if (pID == 0) {
        execlp("./dchat", NULL, (char*) 0);
    }
    sleep(5);
    
    int *result_send;
    int *result_exit;
    int *result_check;
    
    queue = queue_init(message_compare, QUEUE_SIZE);
    

    char * localhostname = (char*) malloc((size_t)INET_ADDRSTRLEN);
    getLocalIp(localhostname);
    
    // create new chat or join one
    if (argc == 3) {
        char remoteHostname = argv[2];
        printf("%s joining an existing chat on %s, listening on %s:%d \n",argv[1],remoteHostname,localhostname,LOCALPORT);
    }
    else{
        printf("%s started a new chat, listening on %s:%d\n", argv[1], localHostname, LOCALPORT);
        
    }
    
    int isSequencer = 1;
    
    userdata.userName = (uname) argv[1];
    userdata.hostname = (hoststr) localHostname;
    userdata.lport = LOCALPORT;
    userdata.leader_flag = isSequencer;
n    
    clientlist = malloc(sizeof(clist));
    clientlist->clientlist.clientlist_len = 0;
    alloc_clients_size = INITIAL_CLIENT_COUNT;
    cname *list_values = (cname*)calloc((size_t)INITIAL_CLIENT_COUNT, sizeof(cname));

    clientlist->clientlist.clientlist_val = list_values;
    
    llist_t* UNSEQ_CHAT_MSGS = (llist_t*) malloc(sizeof(llist_t));
    init_list(UNSEQ_CHAT_MSGS);*/
}
