#include "dchat.h"

/*enum bool
{
  FALSE=0,
  TRUE=1
};
typedef enum bool bool_t;*/
//

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
  strncpy(message->messagebody[newpacket->packetnum*MAXPACKETBODYLEN], newpacket->packetbody, MAXPACKETBODYLEN);

  //check if message is complete
  message->iscomplete = check_chatmessage_completeness(message);

  return message;
}
/*
//add this CHAT packet's contents to the appropriate chat message
//returns whether or not this message is now complete
bool_t append_to_chatmessage(chatmessage_t* message, packet_t* newpacket)
{
  message->packetsreceived[newpacket->packetnum] = TRUE;

  //copy over messagebody 
  strncpy(message->messagebody[newpacket->packetnum*MAXPACKETBODYLEN], newpacket->packetbody, MAXPACKETBODYLEN);

  //check if message is complete
  message->iscomplete = check_chatmessage_completeness(message);

  return message->iscomplete;
}


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

packet* parsePacket(char *buf){
  packet_t* input = malloc(sizeof(packet));
  strcpy(input->sender,strtok(buf,"\n"));
  strcpy(input->uid,strtok(buf,"\n"));
  input->packettype = atoi(strtok(buf,"\n"));
  input->packetnum = atoi(strtok(buf,"\n"));
  input->totalpackets = atoi(strtok(buf,"\n"));
  strcpy(input->packetbody,strtok(buf,"\n"));
  return input;
}

chatmessage_t* find_chatmessage(char[] uid)
{
  node_t* curr = UNSEQ_CHAT_MESSAGES->head;
  while(curr != NULL)
  {
    if(strcmp(uid, ((chatmessage_t*)curr->elem)->uid) == 0)
      return ((chatmessage_t*)curr->elem)
  }
  return NULL;
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
/*    addr.sin_port=htons(LOCALPORT);
    
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

	packet_t* newpacket = parsePacket(&buf);
	
	//figure out what type of packet this is and act accordingly
	switch(newpacket->packettype)
	{
	case CHAT:
	  //figure out if this corresponds to an existing chatmessage
	  chatmessage_t* message = find_chatmessage(newpacket->uid);

	  //if so, and if the message is not complete, add this packet's contents to it
	  //if not, create a new chatmessage
	  bool_t completed = FALSE;
	  if(message)
	    completed = append_to_chatmessage(existingmessage, newpacket);
	  else
	  {
	    message = create_chatmessage(newpacket);
	    completed = newmessage->iscomplete;
	    add_elem(UNSEQ_CHAT_MSGS, (void*)newmessage);
	  }

	  //for now, just print if it's complete
	  if(completed)
	    printf("\E[34m%s\E(B\E[m:\t%s\n", message->sender, message->messagebody);

	  //if am the leader, prep and send a sequencing message for this chatmessage
	  //if not, put it in the unsequenced message list until further notice
	  break;
	case SEQUENCE:
	  //This is a sequencing message. Find the corresponding chat message in the unsequenced message list and enqueue it properly
	  break;
	case CHECKUP:
	  break;
	case ELECTION:
	  break;
	case VOTE:
	  break;
	case VICTORY:
	  break;
	case JOIN_REQUEST:
	  break;
	case LEADER_INFO:
	  break;
	case JOIN:
	  break;
	default:
	  printf("\nUnrecognized packet type: %d\n", newpacket->packettype);
	}









	//begin sequencing stuff from 
	/*
        
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
        */
/*        
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

*/
int main(int argc, char* argv[]){
    
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
