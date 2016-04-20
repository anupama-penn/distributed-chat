#pragma once
#include <pthread.h>

#include "dchat.h"
#include "messagemanagement.h"
#include "clientmanagement.h"

pthread_mutex_t messaging_mutex;

packet_t* parsePacket(char*);
void free_packet(packet_t* packet);
void *receive_UDP(void* t);
void multicast_UDP(packettype_t packettype, char sender[], char senderuid[], char uid[], char messagebody[]);
chatmessage_t* process_packet(chatmessage_t* message, packet_t* newpacket);
void sequence(chatmessage_t* message, packet_t* newpacket);
void assign_sequence(chatmessage_t* message);
void process_late_sequence(chatmessage_t* message, packet_t* newpacket);
void send_UDP(packettype_t packettype, char sender[], char senderuid[], char uid[], char messagebody[], client_t* sendtoclient);
void join_chat(client_t* jointome);
void getLocalIp(char*);