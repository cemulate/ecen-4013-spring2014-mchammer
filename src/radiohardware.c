/*
	Hardware specific code for the MRF24J40 radio on a 2.4GHz
	RF Pictail board on an Explorer16 board.
*/

#include "radiohardware.h"	

void	RadioHW_Init()
{
    RADIO_CS_TRIS = 0;
    RADIO_CS = 1;
    RADIO_RESETn_TRIS = 0;
    RADIO_RESETn = 1;
    RADIO_INT_TRIS = 1;
    RADIO_WAKE_TRIS = 0;
    RADIO_WAKE = 1;

    // PPS

    RPOR0bits.RP0R = 0b00111;       // output SDO1 to RP0
    RPINR20bits.SDI1R = 1;          // input SDI1 to RP1
    RPOR1bits.RP2R = 0b01000;       // output SCK1 to RP2

    #if defined(HARDWARE_SPI)

    SPI1CON1bits.DISSCK = 0;    //Internal Serial Clock is Enabled
    SPI1CON1bits.DISSDO = 0;    //SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 1;    //Communication is word-wide (16 bits)
    SPI1CON1bits.SMP = 0;       //Input Data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 0;       //Serial output data changes on transition from

    SPI1CON1bits.CKP = 0;       //Idle state for clock is a low level;

    SPI1CON1bits.MSTEN = 1;     //Master Mode Enabled
    SPI1STATbits.SPIEN = 1;     //Enable SPI Module
    
    #endif
}
