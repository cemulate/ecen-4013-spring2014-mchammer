#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <xc.h>

#include "cm_uart.h"
#include "cm_radio.h"

#include "radiohardware.h"
#include "MRF24J40.h"

typedef struct {
	WORD	txCnt;				// Counter incremented by sender
	UINT8	data[50];			// Dummy data payload. Make sure to keep sizeof(DATA_PAYLOAD)<=TX_PAYLOAD_SIZE
}DATA_PAYLOAD;

static DATA_PAYLOAD txPayload;	// Structure with data to send.

void RadioInitP2P(void)
// Initialize the tx packet. This only needs to be done once if the framecontrol and dest address isn't
// changed - as here.
{
	Tx.frameControl.frameType = PACKET_TYPE_DATA;
	Tx.frameControl.securityEnabled = 0;
	Tx.frameControl.framePending = 0;
	Tx.frameControl.ackRequest = 1;
	Tx.frameControl.panIDcomp = 1;
	Tx.frameControl.dstAddrMode = SHORT_ADDR_FIELD;
	Tx.frameControl.frameVersion = 0;
	Tx.frameControl.srcAddrMode = NO_ADDR_FIELD;
	Tx.dstPANID = RadioStatus.MyPANID;
	Tx.dstAddr = RadioStatus.MyShortAddress;		// Both nodes for this demo uses the same addresses.
	Tx.payload = (BYTE*)&txPayload;
	Tx.payloadLength=sizeof(DATA_PAYLOAD);
}

int configureRadio(int short_addr, long long long_addr) {

    // *********** PERIPHERAL PIN SELECT ***********
    //
    // The driver code mandates that the PPS and other settings be handled
    // in radiohardware.h/radiohardware.c - so this is where PPS settings 
    // should be changed if needed
    //
    // *********** PERIPHERAL PIN SELECT ***********

    RadioHW_Init();
    uprint("RadioHW_Init done!");

    int radiostatus = RadioInit();
    uprint_int("Radio status is: ", radiostatus);
    RadioInitP2P();

    // Set the short address, long address, and pan ID
    // (I think they can be pretty much anything except the reserved
    // MAC addresses (0's and F's)
    RadioSetAddress(short_addr, long_addr, 0x0F00);

    // Between 11 and 26
    RadioSetChannel(11);

    return radiostatus;

}

void radioSendMessage(char *message, int dest_short_addr) {

    Tx.payloadLength = sprintf((char *)Tx.payload, message);
    Tx.dstAddr = (UINT16)dest_short_addr;

    RadioTXPacket();

    // Blocks until send complete

    while (RadioStatus.TX_BUSY);
    
}

void radioGetMessage(char *buf, int length) {

    while (RadioRXPacket() == 0);           // Block until packet received

    if (Rx.payloadLength > length) {
        Rx.payloadLength = length;
    }

    Rx.payload[Rx.payloadLength] = 0;
    sprintf(buf, (char *)Rx.payload);

    RadioDiscardPacket();

}
