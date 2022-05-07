#include "../include/simulator.h"
#include <iostream>
#include <vector>
#include <string.h>

#define MSGSIZE 20
//#define BUFFERSIZE 1000
#define A 1
#define B 0

using namespace std;

int ackflag, aseq, bseq;
int seq = 0;
int lastsucess = 0;
int numready = 0;
int lastsequence = 0;
vector<pkt> packets;
float timeout = 0.0;
float RTT = 25.0;
vector<float> times;
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

int checksum(struct pkt packet)
{
    char data[MSGSIZE];
    strcpy(data, packet.payload);
    int localchecksum = 0;
    int i = 0;
    while(i < MSGSIZE && data[i] != '\0')
    {
        localchecksum += data[i];
        i++;
    }

    localchecksum += packet.seqnum;
    localchecksum += packet.acknum;

    return localchecksum;   
}

