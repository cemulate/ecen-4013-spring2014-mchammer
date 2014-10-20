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

void radioSendMessage(char *message, int dest_short_addr);
void radioGetMessage(char *buf, int length);

void radioDemo();

#endif	/* CM_RADIO_H */

