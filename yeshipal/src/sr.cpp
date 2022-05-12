
#include "../include/simulator.h"
#include "../include/common.h"
#include <iostream>
#include <string.h>
#include <vector>
/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer 
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  packets.push_back(*createPacket(message));
  if(base == 0)
  {
    next_packet = packets.at(seq);
    tolayer3(0, next_packet);
    timevector.push_back(get_sim_time());
    starttimer(0, RTT);
    seq++;
    base++;
  }
  else if(base < getwinsize())
  {
    next_packet = packets.at(seq);
    tolayer3(0, next_packet);
    base++;
    seq += 1;
  }
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  ack = 1;
  int a = nextseq + 1;
  int b = previous + getwinsize();
  int id = packet.acknum;
  cout << a;
  cout << b;
  if(id == a)
  {
    nextseq++;
  }
  else if(id == b)
  {
    previous += getwinsize();
    stoptimer(0);
  }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
  for (int i = 0; i < sizeof(timevector); i++)
  {
    timeout = get_sim_time() - timevector.at(i);
    
    if(timeout >= RTT)
      {
        tolayer3(0, next_packet);
        starttimer(0, RTT);
      }
  }
  tolayer3(0, next_packet);
  starttimer(0, RTT);
}  
/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
  ack = 1;
  RTT = 25.0;
  timeout = 0.0;
  a_seq = 0;
  seq = 0;
  base = 0;
  nextseq = 0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{

  int a = packet.seqnum;
  int check = packet.checksum;
  if(b_seq == a && checksum(packet) == check)
  {
    tolayer5(1, packet.payload);
    pkt *ACK = new struct pkt;
    (*ACK).acknum = b_seq;
    (*ACK).checksum = packet.seqnum;
    tolayer3(1, *ACK);
    b_seq++;
  }
  else if(b_seq != a && checksum(packet) == check)
  {
    pkt *ACK = new struct pkt;
    (*ACK).acknum = b_seq - 1;
    (*ACK).checksum = packet.seqnum;
    tolayer3(1, *ACK);
  }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
  b_seq = 0;
}
