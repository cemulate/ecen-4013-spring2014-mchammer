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

void configureADC(int analogport) {
    AD1CON1bits.AD12B = 1;              // 12-bit one-channel higher acc. mode
    AD1CON2bits.VCFG = 0;               // AVdd and AVss as + and - ref
    AD1CON3bits.ADCS = 0;               // T_AD = T_CY (sounds about right probably)

    AD1PCFGL = AD1PCFGL & (~(1 << analogport));      // Disable digital function on pin 'analogport'; make it analog

    AD1CHS0bits.CH0SA = analogport;     // Sample on analog pin 'analogport'
    AD1CON2bits.CHPS = 0;               // Convert "channel 0"

    AD1CON1bits.FORM = 0;               // Read buffer in integer form

    AD1CON1bits.ASAM = 0;               // Don't auto sample, manually set the SAMP bit to sample
    AD1CON1bits.SSRC = 0b111;           // Auto convert (conversion is ended when the module decides)

    AD1CON1bits.ADON = 1;               // Enable module
}

void configureIRReceive() {

    T2CON = 0x8000;             // 1:1 prescale, Timer 2 on

    TRISBbits.TRISB4 = 1;       // Set pin RB4 as input
    RPINR7bits.IC1R = 4;        // Set input IC1 to pin RP4

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

    LED_PIN_TRIS = 0;
    LED_PIN_PORT = 0;

}

void configureLightMCU_SPI() {

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

    RPOR4bits.RP8R = 0b00011;   // Assign output U1TX to pin RP8
    RPINR18bits.U1RXR = 6;      // Assign input U1RX to pin RP6


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

    AUDIO_CLK_TRIS = 0;
    AUDIO_DATA_TRIS = 0;
    AUDIO_RESET_TRIS = 0;

    //hmmm
    AUDIO_CLK_PORT = 1;
    AUDIO_DATA_PORT = 1;

    AUDIO_RESET_PORT = 1;
    //__delay_ms(5);    //5ms delay

    AUDIO_RESET_PORT = 0;
    //__delay_ms(5);    //5ms delay

    AUDIO_RESET_PORT = 1;
}