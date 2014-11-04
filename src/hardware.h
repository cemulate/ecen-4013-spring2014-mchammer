/* 
 * File:   hardware.h
 * Author: Chase
 *
 * Created on October 31, 2014, 1:15 PM
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

/*
 * This file centralizes PPS settings, module configuration, and
 * pin mapping into one pair of files (hardware.h/c).
 */

// Specific file for radio related hardware defines/config
#include "radiohardware.h"

#define pADC_PIN_NUM    9
void configureADC();

#define pIRTX_PIN_NUM   4
#define pIRTX_PIN_TRIS  TRISBbits.TRISB4
void configureIRReceive();

#define pLED_PIN_TRIS    TRISBbits.TRISB5
#define pLED_PIN_PORT    PORTBbits.RB5
void configureIRSend();

#define pLIGHTMCU_SDI_RPOR  RPOR0bits.RP0R
#define pLIGHTMCU_SCK_RPOR  RPOR0bits.RP1R
#define pLIGHTMCU_SDO_TRIS  TRISBbits.TRISB0
#define pLIGHTMCU_SDO_PORT  PORTBbits.RB0
#define pLIGHTMCU_SCK_TRIS  TRISBbits.TRISB1
#define pLIGHTMCU_SCK_PORT  PORTBbits.RB1
void configureLightMCU_SPI();

// Pin defines for this are in radiohardware.h
int configureRadio(int short_addr, long long long_addr);

#define pUART_TX_RPOR   RPOR4bits.RP8R
#define pUART_RX_NUM    6
void configureUART1();

void configureTimer1_1600();

#define pAUDIO_CLK_TRIS      TRISAbits.TRISA0
#define pAUDIO_CLK_PORT      PORTAbits.RA0
#define pAUDIO_DATA_TRIS     TRISAbits.TRISA1
#define pAUDIO_DATA_PORT     PORTAbits.RA1
#define pAUDIO_RESET_TRIS    TRISAbits.TRISA2
#define pAUDIO_RESET_PORT    PORTAbits.RA2
void configureAudio();

#endif	/* HARDWARE_H */

