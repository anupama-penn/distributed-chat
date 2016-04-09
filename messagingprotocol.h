//#pragma once
#include "dchat.h"

static int LOCALPORT = DEFAULTPORT;

packet_t* parsePacket(char*);
void *receive_UDP(void* t);
void multicast_UDP(packettype_t packettype, char sender[], char messagebody[]);
void getLocalIp(char*);
