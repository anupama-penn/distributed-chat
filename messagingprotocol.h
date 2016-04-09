#pragma once
#include "dchat.h"
#include "messagemanagement.h"
#include "clientmanagement.h"

packet_t* parsePacket(char*);
void *receive_UDP(void* t);
void multicast_UDP(packettype_t packettype, char sender[], char uid[], char messagebody[]);
void getLocalIp(char*);
