
#include "../include/simulator.h"
#include <iostream>
#include <vector>
#include <string.h>
#define MSGSIZE 20
#define BUFFERSIZE 1000

using namespace std;

int a_seq, b_seq, ack;
int seq;
int previous;
int base;
int nextseq;
vector<pkt> packets;
float RTT;
float timeout;
vector<float> timevector;
struct pkt next_packet;
int checksum(struct pkt);

struct pkt *createPacket(struct msg message)
{
    struct pkt *packet = new struct pkt;
    (*packet).seqnum = seq;
    (*packet).acknum = 0;
    strcpy((*packet).payload, message.data);
    (*packet).checksum = checksum((*packet));
    return packet;
}

int checksum(struct pkt packet_to_send)
{
    char msg_cpy[MSGSIZE];
    int i = 0;
    while(i<MSGSIZE)
    {
      msg_cpy[i] = packet_to_send.payload[i];
      i++;
    }
    strcpy(packet_to_send.payload, msg_cpy);
    int sum = 0;
    for(int i = 0; i<MSGSIZE && msg_cpy[i] != '\0'; i++){
      sum += msg_cpy[i];
    }
    sum = sum + packet_to_send.seqnum;
    sum = sum + packet_to_send.acknum;
    for (int i = 0; i < sizeof(msg_cpy); i++)
        msg_cpy[i] = '\0';
    return sum;
}

