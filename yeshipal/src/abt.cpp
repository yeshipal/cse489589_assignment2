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
#define RTT 20.0

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  
  packets.push_back(*createPacket(message));
    seq++;
    if(ackflag == 1)
    {
        ackflag = 0;
        lastpkt = packets.at(aseq);
        int blah = checksum(lastpkt);
        tolayer3(A, lastpkt);
        starttimer(A, RTT);
    }
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  
    if(packet.acknum == aseq)
    {
        ackflag = 1;
        stoptimer(A);
        aseq++;
    }
    else
    {
        starttimer(A, RTT);
        tolayer3(A, lastpkt);
    }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
    starttimer(A, RTT);
    tolayer3(A, lastpkt);
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
    ackflag = 1;
    aseq = 0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
 
  //int bleh = checksum(packet);
    if(bseq == packet.seqnum && checksum(packet) == packet.checksum)
    {
        tolayer5(B, packet.payload);
        pkt *ACK = new struct pkt;
        (*ACK).acknum = bseq;
        (*ACK).checksum = packet.seqnum;
        tolayer3(B, *ACK);
        //cout << "ACK Checksum: " << checksum(packet) << endl;
        bseq++;
    }
    else if(bseq != packet.seqnum && checksum(packet) == packet.checksum)
    {

        pkt *ACK = new struct pkt;
        (*ACK).acknum = -1;
        (*ACK).checksum = packet.seqnum;
        tolayer3(B, *ACK);
    }
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
    bseq = 0;
}
