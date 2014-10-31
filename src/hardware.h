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

void configureADC(int analogport);

void configureIRReceive();

#define LED_PIN_TRIS    TRISBbits.TRISB5
#define LED_PIN_PORT    PORTBbits.RB5
void configureIRSend();

void configureLightMCU_SPI();

int configureRadio(int short_addr, long long long_addr);

void configureUART1();

void configureTimer1_1600();

#endif	/* HARDWARE_H */

