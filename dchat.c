#include "dchat.h"
#include "messagingprotocol.h"

void error(char *x){
  perror(x);
  exit(1);
}

bool_t check_chatmessage_completeness(chatmessage_t* message)
{
  int max = message->numpacketsexpected;
  int i;
  for(i = 0; i < max; i++)
  {
    if(message->packetsreceived[i] == FALSE)
      return FALSE;
  }
  return TRUE;
}

//create a new chatmessgae given a packet
chatmessage_t* create_chatmessage(packet_t* newpacket)
{
  chatmessage_t* message = malloc(sizeof(chatmessage_t));
  message->seqnum = -1;
  message->numpacketsexpected = newpacket->totalpackets;
  message->iscomplete = FALSE;
  strcpy(message->sender,newpacket->sender);
  strcpy(message->uid,newpacket->uid);

  //indicate which packet has been received
  int i;
  for(i = 0; i < MESSAGEMULTIPLIER; i++)
    message->packetsreceived[i] = FALSE;
  message->packetsreceived[newpacket->packetnum] = TRUE;

  //copy over messagebody 
  strncpy(&message->messagebody[newpacket->packetnum*MAXPACKETBODYLEN], newpacket->packetbody, MAXPACKETBODYLEN);

  //check if message is complete
  message->iscomplete = check_chatmessage_completeness(message);

  return message;
}

//add this CHAT packet's contents to the appropriate chat message
//returns whether or not this message is now complete
bool_t append_to_chatmessage(chatmessage_t* message, packet_t* newpacket)
{
  message->packetsreceived[newpacket->packetnum] = TRUE;

  //copy over messagebody 
  strncpy(&message->messagebody[newpacket->packetnum*MAXPACKETBODYLEN], newpacket->packetbody, MAXPACKETBODYLEN);

  //check if message is complete
  message->iscomplete = check_chatmessage_completeness(message);

  return message->iscomplete;
}

// comparing sequence number of messages to print it acc to total ordering
int message_compare(chatmessage_t* message1, chatmessage_t* message2)
{
  if(message1->seqnum > message2->seqnum)
    return 1;
  else if(message1->seqnum < message2->seqnum)
    return -1;
  return 0;
}

// chack if input is of-> enum msg_type_t {TEXT = 0, NEWUSER = 1, USEREXIT = 2, ELECTION = 3};

chatmessage_t* find_chatmessage(char uid[])
{
  node_t* curr = UNSEQ_CHAT_MSGS->head;
  while(curr != NULL)
  {
    if(strcmp(uid, ((chatmessage_t*)curr->elem)->uid) == 0)
      return ((chatmessage_t*)curr->elem);
    curr = curr->next;
  }
  //return null if it doesn't find what it's looking for
  return NULL;
}



//reimplement using llist_t* CLIENTS of client_t's
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

//incomplete
void holdElection() {
    //Elect a new sequencer
}


// add some way to check if client is alive

bool_t initialize_data_structures() {
    
  init_list(CLIENTS);
  init_list(UNSEQ_CHAT_MSGS);

  INITIALIZED = TRUE;
  return INITIALIZED;
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

client_t* add_client(char username[], char hostname[], int portnum, bool_t isleader)
{
  client_t* newclient = (client_t*)malloc(sizeof(client_t));
  strcpy(newclient->username,username);
  strcpy(newclient->hostname,hostname);
  newclient->portnum = portnum;
  newclient->isleader = isleader;
  add_elem(CLIENTS,(void*)newclient);
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






/*
void shutdown(){
n    
    destroy_data_structures();
    exit(0);
    return NULL;
}

*/

void *get_user_input(void* t)
{
  char userinput[MAXCHATMESSAGELEN];
  while(1)
  {
    scanf("%s",userinput);
    char* sendstr = strdup(userinput);
    multicast_UDP(CHAT,"myname", sendstr);
    free(sendstr);
  }
  pthread_exit((void *)t);
}

void create_message_threads()
{
  int numthreads = 2;
  pthread_t threads[numthreads];
  pthread_attr_t attr;
  void *exitstatus;
  
  //make sure threads are joinable
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  //start a thread for each
  //TODO figure out how to do this simultaneously
  pthread_create(&threads[RECEIVE_THREADNUM], &attr, receive_UDP, (void *)RECEIVE_THREADNUM);
  pthread_create(&threads[SEND_THREADNUM], &attr, get_user_input, (void *)SEND_THREADNUM);

  pthread_join(threads[RECEIVE_THREADNUM], &exitstatus);
  pthread_join(threads[SEND_THREADNUM], &exitstatus);
}

int main(int argc, char* argv[]){
    
  CLIENTS = (llist_t*) malloc(sizeof(llist_t));
  UNSEQ_CHAT_MSGS = (llist_t*) malloc(sizeof(llist_t));
  initialize_data_structures();

  printf("I'm awake.\n");

  //add the other fake guy
  if(strcmp(argv[1],"5000"))
  {
    LOCALPORT = 6000;
    printf("I'm 6000\n");
    add_client("follower\0","127.0.0.1\0",5000,FALSE);
    add_client("follower\0","127.0.0.1\0",6000,FALSE);
    //    receive_UDP();
  }
  else if(strcmp(argv[1],"6000"))
  {
    LOCALPORT = 5000;
    printf("I'm 5000\n");
    add_client("leader\0","127.0.0.1\0",5000,TRUE);
    add_client("leader\0","127.0.0.1\0",6000,TRUE);
    //    multicast_UDP(CHAT, "name", "hello");
  }

  create_message_threads();



  while(1);
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
