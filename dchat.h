#pragma once
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <ncursesw/curses.h>

#include "queue.h"
#include "llist.h"
#include "send_msg.h"
#include "unreliablesplash.h"

#define QUEUE_SIZE 128
#define INITIAL_CLIENT_COUNT 8
#define MSGBUFSIZE 256

#define RECEIVE_THREADNUM 0
#define SEND_THREADNUM 1
#define CHECKUP_THREADNUM 2

#define MAXIPLEN 32
#define MAXSENDERLEN 64
#define MAXUIDLEN 128
#define MAXPACKETLEN 1024
#define MAXPACKETBODYLEN 815// MAXPACKETLEN-MAXSENDERLEN-MAXUIDLEN-(2*sizeof(int))-sizeof(int)-(5*sizeof(char))
#define MESSAGEMULTIPLIER 10
#define MAXCHATMESSAGELEN MAXPACKETBODYLEN*MESSAGEMULTIPLIER

#define PACKETDELIM "\n"

#define DEFAULTPORT 2886

/*enum bool_t
{
  FALSE=0,
  TRUE=1
};
typedef enum bool_t bool_t;*/

enum packettype { CHAT = 0, SEQUENCE = 1, CHECKUP = 2, ELECTION = 3, VOTE = 4, VICTORY = 5, JOIN_REQUEST = 6, LEADER_INFO = 7, JOIN = 8};
typedef enum packettype packettype_t;

typedef struct packet_t {
  char sender[MAXSENDERLEN];
  char uid[MAXUIDLEN];
  packettype_t packettype;
  int packetnum;
  int totalpackets;
  char packetbody[MAXPACKETBODYLEN];
} packet_t;

typedef struct chatmessage_t {
  int seqnum;
  int numpacketsexpected;
  bool iscomplete;
  bool packetsreceived[MESSAGEMULTIPLIER];//indicates which packets have been received
  char sender[MAXSENDERLEN];
  char uid[MAXUIDLEN];
  char messagebody[MAXCHATMESSAGELEN];
} chatmessage_t;

typedef struct client_t {
  char username[MAXSENDERLEN];
  char hostname[MAXIPLEN];
  int portnum;
  bool isleader;
  int missed_checkups;
} client_t;




//int isSequencer = 0;
//int alloc_clients_size;

//clist *clientlist;


//cname userdata;
//static char buf[1024];

llist_t* UNSEQ_CHAT_MSGS;
llist_t* CLIENTS;
queue_t* HBACK_Q; 
client_t* me;
int LOCALPORT;
char* LOCALHOSTNAME;
int SEQ_NO; 
int LEADER_SEQ_NO; 

//int LOCALPORT = DEFAULTPORT;
//static bool INITIALIZED = FALSE;

// Function Declarations

void error(char*);

void *get_user_input(void* t);

void *checkup_on_clients(void* t);

//incomplete
void holdElection();

// add some way to check if client is alive

bool initialize_data_structures();

//void destroy_data_structures();

// keep checking if sequencer is alive
//int check();

//void shutdown();

//UI functions
