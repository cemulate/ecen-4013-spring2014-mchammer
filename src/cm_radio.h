/* 
 * File:   cm_radio.h
 * Author: Chase Meadors
 *
 * Created on October 19, 2014, 2:55 AM
 */

#ifndef CM_RADIO_H
#define	CM_RADIO_H

#define MY_LONG_ADDRESS 	(0x0000DEADBEEF0000ull)     // device MAC address (8 bytes, 64 bit)
#define MY_SHORT_ADDRESS	(0xa500)                    // short (2 byte) 802.15.4 address
#define MY_PAN_ID		(0x0f00)                    // PAN identifier

int configureRadio(int short_addr, long long long_addr);
/*
	Input: 	short_addr: The 16-bit short address to give the radio
			long_addr: The 64-bit long address to give the radio
	Output: 1 if radio was configured successfully, 0 if otherwise
	Effect: Configures PPS and initializes MRF24J40 driver library for
			communication
*/

void radioSendMessage(char *message, int dest_short_addr);
/*
	Input:	message: Null-terminated string to send
			dest_short_addr: The short address of the recipient
	Output: None
	Effect: Communicates to the radio to send a wireless packet with 
			'message' in the payload. Blocks until the radio reports
			that it is no longer transmitting.
*/


void radioGetMessage(char *buf, int length);
/*
	Input:	buf: Buffer to write into
			length: Maximum length to write into buf
	Output:	None
	Effect: Blocks until the radio reports a received message, and copies
			its payload into buf
*/

void radioDemo();

#endif	/* CM_RADIO_H */

