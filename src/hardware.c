#include <stdlib.h>
#include <stdio.h>

#include <xc.h>
#include "hardware.h"

// For certain functions called in the configure... routines
#include "cm_ir.h"
#include "cm_radio.h"
#include "MRF24J40.h"

// There are specific files, radiohardware.h and
// radiohardware.c that specifically deal with the
// wireless radio
#include "radiohardware.h"

void configureADC() {
    AD1CON1bits.AD12B = 1;              // 12-bit one-channel higher acc. mode
    AD1CON2bits.VCFG = 0;               // AVdd and AVss as + and - ref
    AD1CON3bits.ADCS = 0;               // T_AD = T_CY (sounds about right probably)

    AD1PCFGL = AD1PCFGL & (~(1 << pADC_PIN_NUM));      // Disable digital function on pin 'analogport'; make it analog

    AD1CHS0bits.CH0SA = pADC_PIN_NUM;     // Sample on analog pin 'analogport'
    AD1CON2bits.CHPS = 0;               // Convert "channel 0"

    AD1CON1bits.FORM = 0;               // Read buffer in integer form

    AD1CON1bits.ASAM = 0;               // Don't auto sample, manually set the SAMP bit to sample
    AD1CON1bits.SSRC = 0b111;           // Auto convert (conversion is ended when the module decides)

    AD1CON1bits.ADON = 1;               // Enable module
}

void configureIRReceive() {

    T2CON = 0x8000;             // 1:1 prescale, Timer 2 on

    pIRTX_PIN_TRIS = 1;                 // Set the chosen pin as input
    RPINR7bits.IC1R = pIRTX_PIN_NUM;    // Set input IC1 to the chosen pin

    IC1CONbits.ICM = 0b00;      // Disable Input Capture 1 module
    IC1CONbits.ICTMR = 1;       // Select Timer2 as the IC1 Time base
    IC1CONbits.ICI = 0b01;      // Interrupt on every second capture event
    IC1CONbits.ICM = 0b001;     // Re-enable / Generate capture event every edge

    // Enable Capture Interrupt And Timer2
    IPC0bits.IC1IP = 1;         // Setup IC1 interrupt priority level
    IFS0bits.IC1IF = 0;         // Clear IC1 Interrupt Status Flag
    IEC0bits.IC1IE = 1;         // Enable IC1 interrupt

    initIRReceive();

}

void configureIRSend() {

    pLED_PIN_TRIS = 0;
    pLED_PIN_PORT = 0;

}

void configureLightMCU_SPI() {

    pLIGHTMCU_SDO_TRIS = 0;
    pLIGHTMCU_SCK_TRIS = 0;

//    pLIGHTMCU_SDI_RPOR = 0b00111;       // Set output SDO1 to pLIGHTMCU_SDI
//    pLIGHTMCU_SCK_RPOR = 0b01000;       // Set output SCK1 to pLIGHTMCU_SCK
//    TRISBbits.TRISB0 = 0;
//    TRISBbits.TRISB1 = 0;
//
////    SPI1CON1bits.DISSCK = 0;            //Internal Serial Clock is Enabled
////    SPI1CON1bits.DISSDO = 0;            //SDOx pin is controlled by the module
////    SPI1CON1bits.MODE16 = 0;            //Communication is word-wide (16 bits)
////    SPI1CON1bits.SMP = 0;               //Input Data is sampled at the middle of data output time
////    SPI1CON1bits.CKE = 0;               //Serial output data changes on transition from
////
////    SPI1CON1bits.CKP = 0;               //Idle state for clock is a low level;
////
////    SPI1CON1bits.MSTEN = 1;             //Master Mode Enabled
////    SPI1STATbits.SPIEN = 1;             //Enable SPI Module
//
//    SPI1CON1 = 0;
//    SPI1CON1bits.MSTEN = 1;
//    SPI1STATbits.SPIEN = 1;
}

int configureRadio(int short_addr, long long long_addr) {

    RadioHW_Init();

    int radiostatus = RadioInit();
    RadioInitP2P();

    // Set the short address, long address, and pan ID
    // (I think they can be pretty much anything except the reserved
    // MAC addresses (0's and F's)
    RadioSetAddress(short_addr, long_addr, 0x0F00);

    // Between 11 and 26
    RadioSetChannel(11);

    return radiostatus;

}

void configureUART1() {

    pUART_TX_RPOR = 0b00011;   // Assign output U1TX to pin RP8
    RPINR18bits.U1RXR = pUART_RX_NUM;      // Assign input U1RX to pin RP6


    U1MODE = 0x00;

    // U1BRG = (FCY / (16*baud)) - 1; baud = 9600

    U1BRG = 23;

    // Enable UART1 module, and enable transmission on it

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

void configureTimer1() {

    // Configure Timer1 to generate an interrupt at 1600 Hz

    T1CONbits.TON = 0;          // Disable Timer
    T1CONbits.TCS = 0;          // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;        // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11;     // Select 1:256 Prescaler
    TMR1 = 0x00;                // Clear timer register
    PR1 = 9;                    // Load the period value
    IPC0bits.T1IP = 0x01;       // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;          // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;          // Enable Timer1 interrupt
    T1CONbits.TON = 1;          // Start Timer
}

void configureAudio() {

    pAUDIO_CLK_TRIS = 0;
    pAUDIO_DATA_TRIS = 0;
    pAUDIO_RESET_TRIS = 0;

    //hmmm
    pAUDIO_CLK_PORT = 1;
    pAUDIO_DATA_PORT = 1;

    pAUDIO_RESET_PORT = 1;
    //__delay_ms(5);    //5ms delay

    pAUDIO_RESET_PORT = 0;
    //__delay_ms(5);    //5ms delay

    pAUDIO_RESET_PORT = 1;
}