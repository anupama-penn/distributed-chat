//
//  dchat_server.c
//  
//
//  Created by Anupama Kumar on 3/27/16.
//
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dchat.h"

#define INITIAL_CLIENT_COUNT 8
#define MSG_BUF_SIZE 256

void error(char *x){
    perror(x);
    exit(1);
}

// rpc part start

const MAX_MSG_LEN = 512;        /* 64 Bytes */
const MAX_USR_LEN = 32;         /* 4 Bytes  */
const MAX_IP_LEN =  32;		    /* 4 Bytes  */
const BUFLEN = 556;             /* WTF MATE */

typedef string msg_send<MAX_MSG_LEN>;
typedef string uname<MAX_USR_LEN>;
typedef string hoststr<MAX_IP_LEN>;

enum msg_type_t {TEXT = 0, NEWUSER = 1, USEREXIT = 2, ELECTION = 3};
enum status_code {JSUCCESS = 0, JFAILURE = 1, UNAMEINUSE = 2, UNAMEINVALID = 3};


struct cname {
    
    uname userName;
    hoststr hostname;
    int lport;
    int leader_flag;
    
};

typedef struct cname cname;

struct clist {
    cname clientlist<>;
    
};

struct msg_recv {
    msg_send msg_sent;
    uname user_sent;
    unsigned int seq_num;
    msg_type_t msg_type;
};

//rpc part end


clist *clients;
msg_recv *msg_buffer;

char buf[BUFLEN];

int initialized = FALSE;
int alloc_client_size;
int seq_num = 0;


int initialize_data_structures() {
    
    clients = malloc(sizeof(clist));
    clients->clientlist.clientlist_len = 0;
    alloc_client_size = INITIAL_CLIENT_COUNT;
    
    //calloc sets memory value to 0, unlike malloc
    
    cname *list_values  = (cname*) calloc((size_t)INITIAL_CLIENT_COUNT ,sizeof(cnam));
    clients -> clientlist.clientlist_val = list_values;
    
    msg_buffer = malloc(sizeof(msg_recv)*MSG_BUF_SIZE);
    
    initialized = TRUE;
    
    return 0;
    
}

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
}





void send_UDP_packet( msg_type_t msgType , int sequence, uname sending_user, msg_send sending_message){
    
    struct sockaddr_in addr;
    int fd , cnt, i;
    
    fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    if (fd <0) {
        fprintf(stderr,"socket create failed");
        exit(1);
    }
    
    //iterate through client list setting address,port and sendto:
    
    for (i=0; i<clients->clientlist.clientlist_len; i++) {
        memset(&addr, 0, sizeof(addr));
        
        addr.sin_family=AF_INET;
        addr.sin_port=htons(clients->clientlist.clientlist_val[i].lport);
        
        if (inet_aton(clients->clientlist.clientlist_val[i].hostname, &addr.sin_addr)==0) {
            fprintf(stderr, "inet_aton() failed\n");
            exit(1);
        }
        
        //
        sprintf(buf, "%d,%d,%s,%s", msgType, sequence, sending_user, sending_message);
        
        
        if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            fprintf(stderr, "sendto");
            exit(1);
        }
        
    }
    //close(fd);
}

void multicast_clients( uname userName, hoststr hostname, int lport, int leader_flag){
    
    struct sockaddr_in addr;
    int fd, cnt, i;
    
    fd = socket(AF_INET,SOCK_DGRAM,0);
    
    if (fd < 0) {
        fprintf(stderr,"socket create failed");
        exit(1);
    }
    
    for (i=0; i<clientlist.clientlist_len; i++) {
        
        memset(&addr, 0, sizeof(addr));
        
        addr.sin_family=AF_INET;
        addr.sin_port=htons(clients->clientlist.clientlist_val[i].lport);
        
        if (inet_aton(clients->clientlist.clientlist_val[i].hostname, &addr.sin_addr)==0) {
            fprintf(stderr, "inet_aton() failed\n");
            exit(1);
        }
        
        sprintf(buf, "%d,%s,%s,%d,%d", NEWUSER, userName, hostname, lport, leader_flag);
        
        if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            fprintf(stderr, "sendto");
            exit(1);
        }
    }
    //close(fd);
}


void multicast_exit(uname *exitingUser) {
    
    struct sockaddr_in addr;
    int fd, cnt, i;
    
    fd=socket(AF_INET,SOCK_DGRAM,0);
   
    if (fd < 0) {
        fprintf(stderr, "SOCKET CREATION ERROR");
        exit(1);
    }
    
    // iterate through client list setting address,port and sendto:
    
    for (i=0; i<clients->clientlist.clientlist_len; i++) {
        
        memset(&addr, 0, sizeof(addr));
        addr.sin_family=AF_INET;
        addr.sin_port=htons(clients->clientlist.clientlist_val[i].lport);
        
        if (inet_aton(clients->clientlist.clientlist_val[i].hostname, &addr.sin_addr)==0) {
            fprintf(stderr, "inet_aton() failed\n");
            exit(1);
        }
        
        // store in bug the userexit flag and exitingUser and send
        
        sprintf(buf, "%d,%s", USEREXIT, exitingUser);
        
        if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            fprintf(stderr, "sendto");
            exit(1);
        }
    }
    //close(fd);
}


//add to client list

int add( cname *userdata) {
    
    int unameErr;
    status_code status;
    
    if (initialized == FALSE) {
        initialize_data_structures();
    }
    
    if (clients == NULL) {
        status = 1;
    }
   
    // if username returns with an error, set status code to UNAMEINUSE=2:
    //strdup = returns ptr to duplicate string

    clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].userName = (uname) strdup(userdata->userName);
    
    if (clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].userName = NULL) {
        status = 2;
    }
    
    clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].hostname = (hoststr) strdup(userdata->hostname);
    clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].lport = userdata->lport;
    clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].leader_flag = userdata->leader_flag;
    
    // Multicast the client list
    
    multicast_clients(
    
                      clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].userName,
                      clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].hostname,
                      clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].lport,
                      clients->clientlist.clientlist_val[clients->clientlist.clientlist_len].leader_flag
    
                      );
    
    clients->clientlist.clientlist_len++;

    return status;

}


void send_UDP_packet(msg_recv *message) {
    
    int i;
    
    if (initialized == FALSE) {
        initialize_data_structures();
    }
    
    if (message->msg_sent == NULL || strlen(message->msg_sent) == 0 || message->msg_sent[0] == EOF) {
        return(NULL);
    }
    
    msg_buffer[seq_num % MSG_BUF_SIZE].msg_sent = (char *) malloc(sizeof(msg_send)*strlen(message->msg_sent));
    msg_buffer[seq_num % MSG_BUF_SIZE].user_sent = (char *) malloc(sizeof(uname)*strlen(message->user_sent));
    
    strcpy(msg_buffer[seq_num % MSG_BUF_SIZE].msg_sent, message->msg_sent);
    strcpy(msg_buffer[seq_num % MSG_BUF_SIZE].user_sent, message->user_sent);
    
    msg_buffer[seq_num % MSG_BUF_SIZE].seq_num = seq_num;
    msg_buffer[seq_num % MSG_BUF_SIZE].msg_type = message->msg_type;]
    
    // assign seq_num
    // multicast to clients and on fail/retry ->
    //remove client from clist ,
    //multicast exit msg , seq_num
    
    send_UDP_packet(
    
                    msg_buffer[seq_num % MSG_BUF_SIZE].msg_type,
                    msg_buffer[seq_num % MSG_BUF_SIZE].seq_num,
                    msg_buffer[seq_num % MSG_BUF_SIZE].user_sent,
                    msg_buffer[seq_num % MSG_BUF_SIZE].msg_sent
                    
                    );
    seq_num++ ;
    
    
}


// retry getting message

msg_recv *retry( int *seq_num){
    
    return &(msg_buffer[*seq_num %MSG_BUF_SIZE]);
    
}

// exit of a client

void exit(uname *user) {
    
    if (initialized == FALSE) {
        initialize_data_structures();
    }
    
    for (int i=0; i<<clients->clientlist.clientlist_len; i++) {
        if(strcmp(user, clients->clientlist.clientlist_val[i].userName) == 0) {
            if(i<(clients->clientlist.clientlist_len-1)) {
                memmove(&(clients->clientlist.clientlist_val[i]), &(clients->clientlist.clientlist_val[i+1]),
                        sizeof(cname)*clients->clientlist.clientlist_len-1-i);
                break;
            }
        }
        
    }
    
    multicast_exit(user);

}

// keep checking if sequencer is alive

int check(){
    
    if (initialized == FALSE) {
        initialize_data_structures();
    }
    
    int chk = 0;
    chk++;
    
    
    
}


void shutdown(){
    
    destroy_data_structures();
    exit(0);
    return NULL;
}






























