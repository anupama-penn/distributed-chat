#include "dchat.h"
#include "messagingprotocol.h"
#include "clientmanagement.h"
#include "messagemanagement.h"

#include <ifaddrs.h>
void error(char *x){
  perror(x);
  exit(1);
}

bool initialize_data_structures() {
    
  UNSEQ_CHAT_MSGS = (llist_t*) malloc(sizeof(llist_t));
  CLIENTS = (llist_t*) malloc(sizeof(llist_t));
  STRAY_SEQ_MSGS = (llist_t*) malloc(sizeof(llist_t));
  init_list(CLIENTS);
  init_list(UNSEQ_CHAT_MSGS);
  init_list(STRAY_SEQ_MSGS);
  HBACK_Q = init(message_compare,1000);
  failed_quorums = 0;

  return TRUE;
}

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
      if(fgets(userinput, sizeof(userinput), stdin) == NULL)
	exit(1);
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
  pthread_mutex_lock(&election_happening_mutex);
  election_happening = FALSE;
  pthread_mutex_unlock(&election_happening_mutex);
  while(1)
  {
    sleep(CHECKUP_INTERVAL); // Interval between checkups
    
    char uid[MAXUIDLEN];
    get_new_uid(uid);
    multicast_UDP(CHECKUP,me->username, me->uid, uid, "ARE_YOU_ALIVE"); // multicast checkup message to everyone

    pthread_mutex_lock(&CLIENTS->mutex);
    node_t* curr = CLIENTS->head;
    while(curr != NULL && !election_happening)
    {
      // increment everyones counter by one until they respond
      pthread_mutex_lock(&missed_checkups_mutex);
      ((client_t*)curr->elem)->missed_checkups++;
      pthread_mutex_unlock(&missed_checkups_mutex);

      // check if anyone has missed too many checkups
      if (((client_t*)curr->elem)->missed_checkups >= CHECKUP_DEATH_TIMELIMIT)
      {
        //Only leader can trigger quorum to remove dead clients
        if (me->isleader)
        {
          if (check_quorum_on_client_death(((client_t*)curr->elem)->uid))
          {
          //  print_info_with_senderids(((client_t*)curr->elem)->username,"has gone offline",((client_t*)curr->elem)->hostname,((client_t*)curr->elem)->portnum);
            char uid[MAXUIDLEN];
            get_new_uid(uid);
            multicast_UDP(EXIT, me->username, me->uid, uid, ((client_t*)curr->elem)->uid);
          }
        }
        else
        {
          // If me is not the leader then check if the thought-to-be-dead node is
          if (((client_t*)curr->elem)->isleader)
          {
            if (check_quorum_on_client_death(((client_t*)curr->elem)->uid))
            {
              char uid[MAXUIDLEN];
              get_new_uid(uid);
              pthread_mutex_unlock(&CLIENTS->mutex);
              multicast_UDP(EXIT, me->username, me->uid, uid, ((client_t*)curr->elem)->uid);
              get_new_uid(uid);
              multicast_UDP(ELECTION,me->username, me->uid, uid, "INITIATE_ELECTION"); // multicast checkup message to everyone
              pthread_mutex_lock(&election_happening_mutex);
              election_happening = TRUE;
              pthread_mutex_unlock(&election_happening_mutex);
            }
          }
        }
        break;
      }
      curr = curr->next;
    }
    pthread_mutex_unlock(&CLIENTS->mutex);
    if (election_happening)
    {
      holdElection();
    }
  }
  pthread_exit((void *)t);
}

int countVotes() {
  pthread_mutex_lock(&CLIENTS->mutex);
  pthread_mutex_lock(&client_deference_mutex);
  node_t* curr = CLIENTS->head;
  int temp_votes = 0; 
  while(curr != NULL)
  {
    if (strcmp(((client_t*)curr->elem)->deferent_to, me->uid) == 0)
    {
      temp_votes++;
    }
    curr = curr->next;
  }
  pthread_mutex_unlock(&client_deference_mutex);
  pthread_mutex_unlock(&CLIENTS->mutex);
  return temp_votes;
}

void holdElection() {
  me->isCandidate = TRUE;
  coup_propogated = FALSE;
  time_t start;
  start = clock();
  while (election_happening)
  {
    if (me->isCandidate)
    {
      char uid[MAXUIDLEN];
      get_new_uid(uid);
      multicast_UDP(VOTE, me->username, me->uid, uid, "I_SHOULD_LEAD");
    }
    if (countVotes() == CLIENTS->numnodes)
    {
      stage_coup(me->uid);
    }
    else
    {
      if ((clock()-start > ELECTION_TIMEOUT_MS) && (countVotes() > (CLIENTS->numnodes / 2)) )
      {
        // Handle timeout condition
        stage_coup(me->uid);
      }
    }
    usleep(ELECTION_SLEEP_INTERVAL_MS);
  }
  while (!coup_propogated)
  {
   //  printf("Waiting for election results...\n");
     usleep(ELECTION_SLEEP_INTERVAL_MS);
  }
}

void stage_coup(char incoming_power[])
{
  while (!coup_propogated && election_happening)
  {
    char uid[MAXUIDLEN];
    get_new_uid(uid);
    multicast_UDP(VICTORY, me->username, me->uid, uid, incoming_power);
    usleep(ELECTION_SLEEP_INTERVAL_MS);
  }
  return;
}

bool check_quorum_on_client_death(char uid_death_row_inmate[]){

  if (failed_quorums > 3)
  {
    return TRUE;
  }

  num_clients_agree_on_death_call = 0;
  num_clients_disagree_on_death_call = 0;
  char uid[MAXUIDLEN];
  get_new_uid(uid);
  multicast_UDP(QUORUMRESPONSE,me->username, me->uid, uid, uid_death_row_inmate); 

  time_t start;
  start = clock();
  if (CLIENTS->numnodes == 2)
  {
    //It's just me and the dead client so I only need one response
    while (((num_clients_agree_on_death_call + num_clients_disagree_on_death_call) < 1) && (clock()-start < QUORUM_TIMEOUT_MS) )
    {
    }
  }
  else if (CLIENTS->numnodes == 3)
  {
    //It's me, the dead client, and one alive client so I need two responses
    while (((num_clients_agree_on_death_call + num_clients_disagree_on_death_call) < 2) && (clock()-start < QUORUM_TIMEOUT_MS) )
    {
    }
  }
  else
  {
    while (((num_clients_agree_on_death_call + num_clients_disagree_on_death_call) < (CLIENTS->numnodes / 2)) && (clock()-start < QUORUM_TIMEOUT_MS) )
    {
    }
  }

  if ((num_clients_agree_on_death_call - num_clients_disagree_on_death_call) > 0)
  {
    failed_quorums = 0;
    return TRUE;
  }

  failed_quorums++;
  return FALSE;
}

void create_message_threads()
{
  int numthreads = 4;
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
  pthread_create(&threads[FAIRSEQ_THREADNUM], &attr, fair_sequencing, (void *)FAIRSEQ_THREADNUM);
  pthread_create(&threads[CHECKUP_THREADNUM], &attr, checkup_on_clients, (void *)CHECKUP_THREADNUM);

  //pthread_join(threads[RECEIVE_THREADNUM], &exitstatus);
  //pthread_join(threads[SEND_THREADNUM], &exitstatus);
  //  pthread_join(threads[CHECKUP_THREADNUM], &exitstatus);
  //  pthread_join(threads[FAIRSEQ_THREADNUM], &exitstatus);
}
void discover_ip(char ip[]){

    struct ifaddrs* ifAddrStruct;
    struct ifaddrs* ifa;

    char host[NI_MAXHOST];

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
      getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host,NI_MAXHOST,NULL,0,NI_NUMERICHOST);
      printf("\t Interface : <%s>\n",ifa->ifa_name );
      printf("\t  Address : %s\n", host);

      if (ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name,"em1") == 0) 
      {
	  strcpy(ip,host);
	  break;
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}

int main(int argc, char* argv[]){
    
  splash();
  initialize_data_structures();
  char ip[NI_MAXHOST];
  discover_ip(ip);

  UIRUNNING = 0;


  char* localport = argv[1];
  char* runui = argv[argc-1];

  LOCALPORT = atoi(localport);
  UIRUNNING = atoi(runui);
  //  LOCALHOSTNAME = "127.0.0.1";
  LOCALHOSTNAME = ip;
  UID_COUNTER = 0;
  DUMP_BACKLOG = FALSE;
  pthread_mutex_lock(&me_mutex); //so we don't try to access the me variable before it's set
  printf("Connecting Over %s:%s\n",ip,localport);
  printf("ENTER USERNAME: ");
  scanf("%s",LOCALUSERNAME);
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
    add_client(LOCALUSERNAME,LOCALHOSTNAME,LOCALPORT,TRUE);
    SEQ_NO = 0;
    LEADER_SEQ_NO = 0;
    //    add_client("i_am_follower","127.0.0.1",6000,FALSE); //hardcoded
    create_message_threads();
    char uid[MAXSENDERLEN];
    sprintf(uid,"%s:%d",LOCALHOSTNAME,LOCALPORT);
    print_info_with_senderids(LOCALUSERNAME,"has created a new chat session",uid);
    while(1);
    return 0;
  }

  return 0;
}
