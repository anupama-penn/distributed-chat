#include "dchat.h"

bool check_chatmessage_completeness(chatmessage_t*);

//create a new chatmessgae given a packet
chatmessage_t* create_chatmessage(packet_t*);

//add this CHAT packet's contents to the appropriate chat message
//returns whether or not this message is now complete
bool append_to_chatmessage(chatmessage_t*, packet_t*);

// comparing sequence number of messages to print it acc to total ordering
int message_compare(void* message1, void* message2);

chatmessage_t* find_chatmessage(char uid[]);
