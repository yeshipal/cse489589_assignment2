#include "../include/simulator.h"
#include <iostream>
#include <vector>
#include <string.h>

#define MSGSIZE 20
#define BUFFERSIZE 1000
#define AHOST 0
#define BHOST 1

using namespace std;

int ackflag, a_seq, b_seq;
int seq = 0;
int lastsucess = 0;
int numready = 0;
int lastsequence = 0;
vector<pkt> packets;

struct pkt lastpkt;
int checksum(struct pkt);

struct pkt *createPacket(struct msg message)
{
    struct pkt *packet = new struct pkt;
    (*packet).seqnum = seq;
    (*packet).acknum = seq;
    strcpy((*packet).payload, message.data);
    (*packet).checksum = checksum((*packet));
    return packet;
}

int checksum(struct pkt packet_to_send)
{
    char data[MSGSIZE];
    int sum = 0;

    strcpy(data, packet_to_send.payload);
    
    while(int i = 0; i < MSGSIZE && data[i] != '\0'; i++)
    {
        sum += data[i];
    }

    sum += packet_to_send.seqnum;
    sum += packet_to_send.acknum;

    return sum;   
}

