#include "messagingprotocol.h"

packet_t* parsePacket(char* buf){
  packet_t* input = malloc(sizeof(packet_t));
  strcpy(input->sender,strtok(buf,PACKETDELIM));
  strcpy(input->uid,strtok(NULL,PACKETDELIM));
  input->packettype = atoi(strtok(NULL,PACKETDELIM));
  input->packetnum = atoi(strtok(NULL,PACKETDELIM));
  input->totalpackets = atoi(strtok(NULL,PACKETDELIM));
  strcpy(input->packetbody,strtok(NULL,PACKETDELIM));
  return input;
}

void *receive_UDP(void* t)
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
    //    printf("Binding %d\n",LOCALPORT);
    
    //bind to receive address
    
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
    
    
    while (1) {
        addrlen = sizeof(addr);
        
        nbytes = recvfrom(fd,buf,MAXPACKETLEN,0,(struct sockaddr *) &addr,&addrlen);
        
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
	    printf("\E[34m%s\E(B\E[m (not sequenced):\t%s\n", message->sender, message->messagebody);

	  //if am the leader, prep and send a sequencing message for this chatmessage
	  if(me->isleader)
	  {
	    char seqnum[5];
	    sprintf(seqnum,"%d",LEADER_SEQ_NO);
	    int timestamp = (int)time(NULL);
	    char uid[MAXUIDLEN];
	    sprintf(uid,"%d",timestamp);
	    multicast_UDP(SEQUENCE,me->username,uid,seqnum);
	    LEADER_SEQ_NO++;
	  }
	  break;
	case SEQUENCE:
	  //This is a sequencing message. Find the corresponding chat message in the unsequenced message list and enqueue it properly
	  message = find_chatmessage(newpacket->uid);
	  //If the corresponding message is not complete, ask the leader for its missing part first. It will be received as a chat. TODO
	  message->seqnum = atoi(newpacket->packetbody);
	  q_enqueue(HBACK_Q,(void*)message);
	  chatmessage_t* firstmessage = (chatmessage_t*)q_peek(HBACK_Q);
	  if(firstmessage->seqnum <= SEQ_NO)
	  {
	    SEQ_NO = firstmessage->seqnum + 1;
	    printf("\E[34m%s\E(B\E[m (sequenced: %d):\t%s\n", firstmessage->sender, firstmessage->seqnum,firstmessage->messagebody);
	    q_dequeue(HBACK_Q);
	  }

	  //check if the front of the queue corresponds to our expected current sequence no. If so, print it. If not, we should wait or eventually ping the leader for it.

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
	  //message from someone who wants to join

	  break;
	case LEADER_INFO:
	  //if someone asked to join, but they didn't ask the leader, instead of sending a JOIN, send them this. 
	  //If you receive this, repeat the JOIN_REQUEST, but to the leader. 
	  break;
	case JOIN:
	  //announcement that someone has successfully joined

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
    pthread_exit((void *)t);
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


void multicast_UDP(packettype_t packettype, char sender[], char uid[], char messagebody[]){
    
    struct sockaddr_in addr;
    int fd;
    //    printf("prepping to send\n");

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

    //    printf("total packets required: %d\n", totalpacketsrequired);
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

	  sprintf(packetbuf, "%s%s%s%s%d%s%d%s%d%s%s", sender, PACKETDELIM, uid, PACKETDELIM, packettype, PACKETDELIM, i, PACKETDELIM, totalpacketsrequired, PACKETDELIM, packetbodybuf);
	  //	  printf("now sending %s to %s:%d\n",packetbuf, ((client_t*)curr->elem)->hostname, ((client_t*)curr->elem)->portnum);
	  if (sendto(fd, packetbuf, sizeof(packetbuf), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            fprintf(stderr, "sendto");
            exit(1);
	  }
	}
	curr = curr->next;
    }
    //close(fd);
}

