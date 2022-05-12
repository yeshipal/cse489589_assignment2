
#include "../include/simulator.h"
#include "../include/common.h"
#include <iostream>
#include <vector>
#include <string.h>

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
  seq++;
  if(ack == 1)
  {
    ack = 0;
    next_packet = packets.at(a_seq);
    tolayer3(0, next_packet);
    starttimer(0, timeout);
  }
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  
  if(packet.acknum == a_seq)
  {
    ack = 1;
    stoptimer(0);
    a_seq++;
  }
  else 
  {
    starttimer(0, timeout);
    tolayer3(0, next_packet);
  }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
  tolayer3(0, next_packet);
  starttimer(0, timeout);

}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
  ack = 1;
  timeout = 20.0;
  a_seq = 0;
  seq = 0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
  int a = packet.seqnum;
  int check = packet.checksum;
  int check_verify = checksum(packet);
  if(b_seq == a && check_verify == check)
    {
      tolayer5(1, packet.payload);
      pkt *ACK = new struct pkt;
      (*ACK).acknum = b_seq;
      (*ACK).checksum = packet.seqnum;
      tolayer3(1, *ACK);
      b_seq++;
    }

    else if(b_seq != a && check_verify == check)
    {

      pkt *ACK = new struct pkt;
      (*ACK).acknum = -1;
      (*ACK).checksum = packet.seqnum;
      tolayer3(1, *ACK);
    }
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
    b_seq = 0;
}
