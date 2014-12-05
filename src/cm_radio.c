#include "Common.h"
#include "HammerState.h"
#include "hardware.h"
#include "radiohardware.h"

#include "cm_uart.h"
#include "cm_radio.h"

#include "MRF24J40.h"

#include <string.h>

typedef struct {
	WORD	txCnt;				// Counter incremented by sender
	UINT8	data[50];			// Dummy data payload. Make sure to keep sizeof(DATA_PAYLOAD)<=TX_PAYLOAD_SIZE
}DATA_PAYLOAD;

static DATA_PAYLOAD txPayload;	// Structure with data to send.

void RadioInitP2P()
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
