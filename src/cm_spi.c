#include <stdlib.h>
#include <string.h>

#include <xc.h>

#include "cm_spi.h"

void configureSPI1Master() {

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
    if (SPI1STATbits.SPIROV == 1) {
        uart1_puts("\r\nRX Overflow!!");
        SPI1STATbits.SPIROV = 0;
    }
    return SPI1BUF;

}