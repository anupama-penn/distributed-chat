#include "messagemanagement.h"

bool check_chatmessage_completeness(chatmessage_t* message)
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
  message->messagetype = newpacket->packettype;
  message->seqnum = -1;
  message->numpacketsexpected = newpacket->totalpackets;
  message->iscomplete = FALSE;
  strcpy(message->sender,newpacket->sender);
  strcpy(message->uid,newpacket->uid);
  strcpy(message->senderuid, newpacket->senderuid);
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
bool append_to_chatmessage(chatmessage_t* message, packet_t* newpacket)
{
  message->packetsreceived[newpacket->packetnum] = TRUE;

  //copy over messagebody 
  strncpy(&message->messagebody[newpacket->packetnum*MAXPACKETBODYLEN], newpacket->packetbody, MAXPACKETBODYLEN);

  //check if message is complete
  message->iscomplete = check_chatmessage_completeness(message);

  return message->iscomplete;
}

// comparing sequence number of messages to print it acc to total ordering
int message_compare(void* message1, void* message2)
{
  if(((chatmessage_t*)message1)->seqnum > ((chatmessage_t*)message2)->seqnum)
    return 1;
  else if(((chatmessage_t*)message1)->seqnum < ((chatmessage_t*)message2)->seqnum)
    return -1;
  return 0;
}

// chack if input is of-> enum msg_type_t {TEXT = 0, NEWUSER = 1, USEREXIT = 2, ELECTION = 3};

chatmessage_t* find_chatmessage(char uid[])
{
  pthread_mutex_lock(&UNSEQ_CHAT_MSGS->mutex);
  node_t* curr = UNSEQ_CHAT_MSGS->head;
  while(curr != NULL)
  {
    //    printf("SEQUENCE uid:%s\tMESSAGE uid:%s\n",uid,((chatmessage_t*)curr->elem)->uid);
    if(strcmp(uid, ((chatmessage_t*)curr->elem)->uid) == 0)
    {
      pthread_mutex_unlock(&UNSEQ_CHAT_MSGS->mutex);
      return ((chatmessage_t*)curr->elem);
    }
    curr = curr->next;
  }
  //return null if it doesn't find what it's looking for
  pthread_mutex_unlock(&UNSEQ_CHAT_MSGS->mutex);
  return NULL;
}

