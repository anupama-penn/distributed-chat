//
//  dchat.c
//  
//
//  Created by Anupama Kumar on 3/22/16.
//
//

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

#include "dchat.h"
#include "holdback_queue.h"

#define QUEUE_SIZE 128
#define INITIAL_CLIENT_COUNT 8
#define MSGBUFSIZE 256


void error(char *x){
    perror(x);
    exit(1);
}


// Function Declarations
void print(clist *); // print client list
void getLocalIp(char*);
void holdElection();

int isSequencer = 0;
int alloc_clients_size;

clist *clientlist;

const int LOCALPORT = 2886;
cname userdata;
char buf[BUFLEN];

// comparing sequence number of messages to print it acc to total ordering
int message_compare(const void*message1, const void*message2){
    
    if( ((msg_recv*)message1)->seq_num > ((msg_recv*)message2)->seq_num ) {
        return 1;
    }
    else if ((msg_recv*)message1)->seq_num < ((msg_recv*)message2)->seq_num ){
        return -1;
    }
    else return 0;
}


// chack if input is of-> enum msg_type_t {TEXT = 0, NEWUSER = 1, USEREXIT = 2, ELECTION = 3};

msg_recv* parseMessage(char *buf){
    
    msg_recv* input = malloc(sizeof(msg_recv));
    
    if ((*input).msg_type == TEXT) {
        
        
      //  (*input).seq_num =
      //  (*input).user_sent =
      //  (*input).msg_sent =
        
        return input;
    }
    
    if ((*input).msg_type == NEWUSER) {
       // statements;
    }
    
    
    if ((*input).msg_type == USEREXIT) {
       // statements;
    }
    
    if ((*input).msg_type == ELECTION) {
        // statements;
    }
    
}

void receive_UDP_packet(){
    
    
    int squence = -1;
    
    struct sockaddr_in addr;
    int fd, nbytes,addrlen;

    fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        exit(1);
    }
    
    //setup destination address
    
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
    addr.sin_port=htons(LOCALPORT);
    
    //bind to receive address
    
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
    
    
    while (1) {
        addrlen = sizeof(addr);
        
        nbytes = recvfrom(fd,buf,MSGBUFSIZE,0,(struct sockaddr *) &addr,&addrlen);
        
        if (nbytes <0) {
            perror("recvfrom");
            exit(1);
        }
        
        msg_recv* message_got = parseMessage(&buf);
        
        enqueue(queue, message_got);
        
        msg_recv* next_message_got = dequeue(queue);
        
        if (squence = -1) {
            squence = (*next_message_got).seq_num;
        }
        else if((*next_message_got).seq_num > squence){
            int targetMessage = (*next_message_got).seq_num;
            squence ++;
        
            while (squence <targetMessage) {
            
                printf("redelivery of messages is requested");
            
                next_message_got = retry(&squence);
            
                printf("%s: %s\n", (*next_message_got).user_sent, (*next_message_got).msg_sent);
            
                squence++;
            }
        }
        
        squence ++;
        printf("%s: %s\n", (*next_message_got).user_sent, (*next_message_got).msg_sent);
        
        
    }//end of while
    
}

// incomplete
// discover IP address using name
void getLocalIp(char *buf){
    
    bzero(buf,1024);
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // connect?
    
    struct sockaddr_in sockname;
    socklen_t socknamelen = sizeof(sockname);
    err = getsockname(sock, (struct sockaddr*) &sockname, &socknamelen);

    const char* p = inet_ntop(AF_INET, &sockname.sin_addr, buf, INET_ADDRSTRLEN);
    close(sock);
  
}

void print_client_list(clist * client_list) {
    int numClients = client_list->clientlist.clientlist_len, i;
    
    for (i=0; i<numClients; i++) {
        printf("%s %s: %d",client_list->clientlist.clientlist_val[i].userName,
               client_list->clientlist.clientlist_val[i].hostname,
               client_list->clientlist.clientlist_val[i].lport);
        
        if (client_list->clientlist.clientlist_val[i].leader_flag == 1) {
            printf(" (Leader)");
        }
        printf("\n");
    }
}

//incomplete
void holdElection() {
    //Elect a new sequencer
}


// add some way to check if client is alive


int main(int argc, char* argv[]){
    
    pid_t pID = fork();
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
    
    clientlist = malloc(sizzeof(clist));
    clientlist->clientlist.clientlist_len = 0;
    alloc_clients_size = INITIAL_CLIENT_COUNT;
    cname *list_values = (cname*)calloc((size_t)INITIAL_CLIENT_COUNT, sizeof(cname));
    
    clientlist->clientlist.clientlist_val = list_values;
    
    
}
