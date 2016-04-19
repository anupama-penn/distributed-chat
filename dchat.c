#include "dchat.h"
#include "messagingprotocol.h"
#include "clientmanagement.h"
#include "messagemanagement.h"

#include <ifaddrs.h>
void error(char *x){
  perror(x);
  exit(1);
}

// add some way to check if client is alive

bool initialize_data_structures() {
    
  UNSEQ_CHAT_MSGS = (llist_t*) malloc(sizeof(llist_t));
  CLIENTS = (llist_t*) malloc(sizeof(llist_t));
  STRAY_SEQ_MSGS = (llist_t*) malloc(sizeof(llist_t));
  init_list(CLIENTS);
  init_list(UNSEQ_CHAT_MSGS);
  init_list(STRAY_SEQ_MSGS);
  HBACK_Q = init(message_compare,1000);

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
check    return NULL;
}

*/

void get_new_uid(char uid[])
{
  pthread_mutex_lock(&counter_mutex);
  int timestamp = (int)time(NULL);
  sprintf(uid,"%d(^_^)%d",timestamp,UID_COUNTER);
  UID_COUNTER++;
  pthread_mutex_unlock(&counter_mutex);
}

void send_chat_message(int counter, char userinput[])
{
   char* sendstr = strdup(userinput);
   char uid[MAXUIDLEN];
   get_new_uid(uid);
   multicast_UDP(CHAT,me->username, me->uid, uid, sendstr);
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
      printf("userinput: ");
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

  int counter = 1;
  while(1)
  {
    sleep(CHECKUP_INTERVAL); // Interval between checkups
    
    char uid[MAXUIDLEN];
    get_new_uid(uid);
    multicast_UDP(CHECKUP,me->username, me->uid, uid, "ARE_YOU_ALIVE"); // multicast checkup message to everyone

    pthread_mutex_lock(&CLIENTS->mutex);
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
      if (((client_t*)curr->elem)->missed_checkups == 4)
      {
	print_info_with_senderids(((client_t*)curr->elem)->username,"has gone offline",((client_t*)curr->elem)->hostname,((client_t*)curr->elem)->portnum);
      }

      curr = curr->next;
    }
    pthread_mutex_unlock(&CLIENTS->mutex);
    counter++;
  }
  pthread_exit((void *)t);
}

void create_message_threads()
{
  int numthreads = 3;
  pthread_t threads[numthreads];
  pthread_attr_t attr;
  //  void *exitstatus;
  
  //make sure threads are joinable
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  //start a thread for each
  pthread_mutex_lock(&messaging_mutex); //This gets unlocked in receive_UDP
  pthread_mutex_lock(&initui_mutex); //this gets unlocked in initui
  pthread_create(&threads[SEND_THREADNUM], &attr, get_user_input, (void *)SEND_THREADNUM);
  pthread_create(&threads[RECEIVE_THREADNUM], &attr, receive_UDP, (void *)RECEIVE_THREADNUM);
  pthread_mutex_lock(&messaging_mutex); //Can only get this lock if receive_UDP has unlocked it
  pthread_mutex_unlock(&messaging_mutex);
  pthread_create(&threads[CHECKUP_THREADNUM], &attr, checkup_on_clients, (void *)CHECKUP_THREADNUM);

  //pthread_join(threads[RECEIVE_THREADNUM], &exitstatus);
  //pthread_join(threads[SEND_THREADNUM], &exitstatus);
  //  pthread_join(threads[CHECKUP_THREADNUM], &exitstatus);
}
void discover_ip(){

    struct ifaddrs * ifAddrStruct = NULL;
    struct ifaddrs * ifa=NULL;

    char host[NI_MAXHOST];
    int s ;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host,NI_MAXHOST,NULL,0,NI_NUMERICHOST);

        //replace en0 with em1 interface in spec/eniac

        if (ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name,"en0") == 0) {

            printf("\t  Address : %s\n", host);
            printf("\t Interface : <%s>\n",ifa->ifa_name );

        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}

int main(int argc, char* argv[]){
    
  initialize_data_structures();

  UIRUNNING = 0;

  printf("ENTER USERNAME: ");
  scanf("%s",LOCALUSERNAME);

  char* localport = argv[1];
  char* runui = argv[argc-1];
  printf("I'm awake.\n");

  LOCALPORT = atoi(localport);
  printf("I'm still awake.\n");

  UIRUNNING = atoi(runui);
  printf("I'm extra awake.\n");
  LOCALHOSTNAME = "127.0.0.1";
  UID_COUNTER = 0;
  if(argc == 5)
  {
    SEQ_NO = -1;
    char* remoteip = argv[2];
    char* remoteport = argv[3];
    client_t* jointome = create_client("",remoteip,atoi(remoteport),TRUE);
    create_message_threads();
    join_chat(jointome);
    while(1);
    return 0;
  }
  else
  {
    /*    if(LOCALPORT == 6000)
    {
      add_client("i_am_leader","127.0.0.1",5000,TRUE);
      add_client("i_am_follower","127.0.0.1",6000,FALSE);
      //      SEQ_NO = 0;
      //      LEADER_SEQ_NO = 0;
      create_message_threads();
      while(1);
      return 0;
      }*/
    add_client(LOCALUSERNAME,LOCALHOSTNAME,LOCALPORT,TRUE);
    SEQ_NO = 0;
    LEADER_SEQ_NO = 0;
    //    add_client("i_am_follower","127.0.0.1",6000,FALSE); //hardcoded
    create_message_threads();
    print_info_with_senderids(LOCALUSERNAME,"has created a new chat session",LOCALHOSTNAME,LOCALPORT);
    while(1);
    return 0;
  }

  /*
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
*/
  return 0;
}
