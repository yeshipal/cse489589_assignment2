#include "../include/simulator.h"
#include <iostream>
#include <vector>
#include <string.h>
#define MSGSIZE 20
#define BUFFERSIZE 1000

using namespace std;

int ack, a_seq, b_seq;
int seq;
int previous;
int base = 0;
int nextseq = 0;
vector<pkt> packets;
float RTT;
float timeout;
vector<float> times;
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
    /*char data[MSGSIZE];
    strcpy(data, packet_to_send.payload);
    int sum = 0;
    int i = 0;
    while(i < MSGSIZE && data[i] != '\0')
    {
        sum += data[i];
        i++;
    }

    sum += packet_to_send.seqnum;
    sum += packet_to_send.acknum;

    return sum;   */
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

