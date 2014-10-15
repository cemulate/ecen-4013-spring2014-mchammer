/* 
 * File:   main.c
 * Author: Chase
 *
 * Created on October 13, 2014, 3:46 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <p24HJ64GP502.h>

// --------------------- Configuration bits ---------------------

// Configure clock settings:

#pragma config FNOSC = FRC
// Oscillator Mode (Internal Fast RC (FRC) with divide by N)
// This is the default clock. The FRC oscillates at a nominal 7.37 MHz,
// And "N" is by default 1, leaving our processor clocked at 7.37 MHz

#pragma config IESO = OFF
// Internal External Switch Over Mode = OFF
// Tells the processor to exclusively use the oscillator chosen above
// when starting up

// For previous two, see PIC24HJ64GP502 Datasheet sec. 9

#pragma config ALTI2C = OFF
// Tells the PIC to map I2C pins to ASDA1 and ASCL1, as opposed to
// the originals - SDA1 and SCL1. Datasheet sec. 25.1

#pragma config JTAGEN = OFF
// Disable JTAG (we don't need it = less pins taken up). Datasheet sec. 25.1

#pragma config OSCIOFNC = ON
// Tell the OSC2 pin to behave as a regular digital I/O pin, instead of
// exporting the internal clock signal. Datasheet sec 25.1

// --------------------------------------------------------------

#include "util/uart.h"

void blinkForever();
void blinkCommandLine();

int main(int argc, char** argv) {

    TRISAbits.TRISA2 = 0;       // Set pin RA2 to be digital output
    PORTAbits.RA2 = 1;          // Set pin RA2 high

    // REFERENCE: Datasheet sec 11.1

    RPOR4bits.RP8R = 0b00011;   // Assign output U1TX to pin RP2
    RPINR18bits.U1RXR = 6;      // Assign input U1RX to pin RP3

    // REFERENCE: Datasheet sec 11.6.2.2 and 11.6.2.1 respectively,
    // This is using Peripheral Pin Select (PPS)

    configureUART1();           // Set up UART1 module w/ baud 9600

    blinkCommandLine();

    return (EXIT_SUCCESS);
}

void blinkForever() {
    long i = 0;
    while (1) {
        i = 300000;
        while (i--) {

        }
        PORTAbits.RA2 = ~PORTAbits.RA2;
        uart1Tx("Blink...\r\n");
    }
}

void blinkCommandLine() {

    char rxbuf[2];
    char confirm_message[50];
    int count = 0;
    long i = 0;
    while (1) {
        // Set RA2 low
        PORTAbits.RA2 = 0;

        uart1Tx("\r\n\r\n");
        uart1Tx("Please enter a number between 1 and 9: ");

        uart1Rx(rxbuf, 1);

        uart1Tx("\r\n");

        int number = rxbuf[0] - 48;

        sprintf(confirm_message, "Blinking %d times ... ", number);
        uart1Tx(confirm_message);

        for (count = 0; count < number; count ++) {

            for (i = 0; i < 30000; i ++) {

            }

            PORTAbits.RA2 = ~PORTAbits.RA2;

            for (i = 0; i < 30000; i ++) {

            }

            PORTAbits.RA2 = ~PORTAbits.RA2;

        }
    }
}
