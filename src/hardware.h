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

void configureLightMCU_SPI();

// Pin defines for this are in radiohardware.h
int configureRadio(int short_addr, long long long_addr);

#define pUART_TX_RPOR   RPOR4bits.RP8R
#define pUART_RX_NUM    6
void configureUART1();

void configureTimer1_1600();

#define pAUDIO_CLK_TRIS      TRISBbits.TRISB7
#define pAUDIO_CLK_PORT      PORTBbits.RB7
#define pAUDIO_DATA_TRIS     TRISBbits.TRISB9
#define pAUDIO_DATA_PORT     PORTBbits.RB9
#define pAUDIO_RESET_TRIS    TRISBbits.TRISB10
#define pAUDIO_RESET_PORT    PORTBbits.RB10
void configureAudio();

#endif	/* HARDWARE_H */

