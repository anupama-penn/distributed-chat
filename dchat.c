#include "dchat.h"

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

packet_t* parsePacket(char* buf){
  packet_t* input = malloc(sizeof(packet_t));
  strcpy(input->sender,strtok(buf,PACKETDELIM));
  strcpy(input->uid,strtok(buf,PACKETDELIM));
  input->packettype = atoi(strtok(buf,PACKETDELIM));
  input->packetnum = atoi(strtok(buf,PACKETDELIM));
  input->totalpackets = atoi(strtok(buf,PACKETDELIM));
  strcpy(input->packetbody,strtok(buf,PACKETDELIM));
  return input;
}

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

void receive_UDP_packet()
{
    
    struct sockaddr_in addr;
    int fd;
    int nbytes;
    socklen_t addrlen;
    char buf[MAXPACKETLEN];

    //fd = socket(PF_INET,SOCK_DGRAM,0);
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
        
	printf("Waiting...\n");
        nbytes = recvfrom(fd,buf,MAXPACKETLEN,0,(struct sockaddr *) &addr,&addrlen);
        
	printf("I got something!\t%d bytes\n",nbytes);
        if (nbytes <0) {
            perror("recvfrom");
            exit(1);
        }

	packet_t* newpacket = parsePacket(buf);
	chatmessage_t* message;
	bool_t completed = FALSE;
	
	//figure out what type of packet this is and act accordingly
	switch(newpacket->packettype)
	{
	case CHAT:
	  //figure out if this corresponds to an existing chatmessage
	  message = find_chatmessage(newpacket->uid);

	  //if so, and if the message is not complete, add this packet's contents to it
	  //if not, create a new chatmessage
	  if(message)
	    completed = append_to_chatmessage(message, newpacket);
	  else
	  {
	    message = create_chatmessage(newpacket);
	    completed = message->iscomplete;
	    add_elem(UNSEQ_CHAT_MSGS, (void*)message);
	  }

	  //for now, just print if it's complete
	  if(completed)
	    printf("\E[34m%s\E(B\E[m:\t%s\n", message->sender, message->messagebody);

	  //if am the leader, prep and send a sequencing message for this chatmessage
	  //if not, put it in the unsequenced message list until further notice
	  break;
	case SEQUENCE:
	  //This is a sequencing message. Find the corresponding chat message in the unsequenced message list and enqueue it properly
	  //If the corresponding message is not complete, ask the leader for its missing part first. It will be received as a chat. 
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
    int err = getsockname(sock, (struct sockaddr*) &sockname, &socknamelen);


    const char* p = inet_ntop(AF_INET, &sockname.sin_addr, buf, INET_ADDRSTRLEN);
    close(sock);
    return;
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



void multicast_UDP( packettype_t packettype, char sender[], char messagebody[]){
    
    struct sockaddr_in addr;
    int fd;
    printf("prepping to send\n");

    int totalpacketsrequired = (strlen(messagebody)) / 815; 
    int remainder =  strlen(messagebody) % MAXPACKETBODYLEN; 
    if(remainder > 0)
      totalpacketsrequired++;
    
    fd = socket(PF_INET,SOCK_DGRAM,0);
    
    if (fd < 0) {
        fprintf(stderr,"socket create failed");
        exit(1);
    }
    
    node_t* curr = CLIENTS->head;
    char packetbodybuf[MAXPACKETBODYLEN];
    char packetbuf[MAXPACKETLEN];
    
    int timestamp = (int)time(NULL);
    char uid[MAXUIDLEN];
    sprintf(uid,"%d",timestamp);


    printf("total packets required: %d\n", totalpacketsrequired);
    while(curr != NULL)
    {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family=AF_INET;
        addr.sin_port=htons(((client_t*)curr->elem)->portnum);
        
	if (inet_aton(((client_t*)curr->elem)->hostname, &addr.sin_addr)==0) {
	  fprintf(stderr, "inet_aton() failed\n");
	  exit(1);
	}

	int messageindex = 0;
	int i;
	for(i = 0; i < totalpacketsrequired; i++)
	{
	  strncpy(packetbodybuf, messagebody+messageindex, MAXPACKETBODYLEN);
	  messageindex += MAXPACKETBODYLEN;

	  sprintf(packetbuf, "%s%s%s%s%d%s%d%s%d%s%s", sender, PACKETDELIM, uid, PACKETDELIM, packettype, PACKETDELIM, i, PACKETDELIM, totalpacketsrequired, PACKETDELIM, messagebody);
	  printf("now sending %s to %s:%d\n",packetbuf, ((client_t*)curr->elem)->hostname, ((client_t*)curr->elem)->portnum);
	  if (sendto(fd, packetbuf, sizeof(packetbuf), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            fprintf(stderr, "sendto");
            exit(1);
	  }
	}
	printf("sent?\n");
	curr = curr->next;
    }
    //close(fd);
}




/*
void shutdown(){
    
    destroy_data_structures();
    exit(0);
    return NULL;
}

*/
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
    receive_UDP_packet();
  }
  else if(strcmp(argv[1],"6000"))
  {
    LOCALPORT = 5000;
    printf("I'm 5000\n");
    add_client("leader\0","127.0.0.1\0",6000,TRUE);
    multicast_UDP(CHAT, "name", "hello");
  }



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
