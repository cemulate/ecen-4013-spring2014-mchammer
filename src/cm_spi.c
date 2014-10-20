#include <stdlib.h>
#include <string.h>

#include <xc.h>

#include "cm_spi.h"
#include "cm_uart.h"

void configureSPI1Master() {

    // *********** PERIPHERAL PIN SELECT ***********
    // Change this if needed

    RPOR0bits.RP0R = 0b00111;       // Set output SDO1 to RP0
    RPOR0bits.RP1R = 0b01000;       // Set output SCK1 to RP1
    RPOR1bits.RP2R = 0b01001;       // Set output SS1 to RP2

    // *********** PERIPHERAL PIN SELECT ***********

    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0;    //Internal Serial Clock is Enabled
    SPI1CON1bits.DISSDO = 0;    //SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 1;    //Communication is word-wide (16 bits)
    SPI1CON1bits.SMP = 0;       //Input Data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 0;       //Serial output data changes on transition from

    SPI1CON1bits.CKP = 0;       //Idle state for clock is a low level;

    SPI1CON1bits.MSTEN = 1;     //Master Mode Enabled
    SPI1STATbits.SPIEN = 1;     //Enable SPI Module
}

void configureSPI1Slave() {

    // *********** PERIPHERAL PIN SELECT ***********
    // Change this if needed

    RPINR20bits.SDI1R = 3;          // Set input SDI1 to RA3
    RPINR20bits.SCK1R = 4;          // Set input SCK1 to RA4
    RPINR21bits.SS1R = 5;           // Set intput SS1 to RA5

    // *********** PERIPHERAL PIN SELECT ***********

    SPI1BUF = 0;

    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0;    //Internal Serial Clock is Enabled
    SPI1CON1bits.DISSDO = 1;    //SDOx pin is unused by the module
    SPI1CON1bits.MODE16 = 1;    //Communication is word-wide (16 bits)
    SPI1CON1bits.SMP = 0;       //Input Data is sampled at the middle of data
    SPI1CON1bits.SSEN = 0;

    SPI1CON1bits.CKE = 0;       //Serial output data changes on transition

    SPI1CON1bits.CKP = 0;       //Idle state for clock is a low level; active

    SPI1CON1bits.MSTEN = 0;     //Master Mode disabled
    SPI1STATbits.SPIROV = 0;    //No Receive Overflow has Occurred
    SPI1STATbits.SPIEN = 1;     //Enable SPI Module
}

void spi1Tx(int data) {

    while (SPI1STATbits.SPITBF);
    SPI1BUF = data;

}

int spi1Rx() {

    while (SPI1STATbits.SPIRBF == 0);
    return SPI1BUF;

}