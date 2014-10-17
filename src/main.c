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

#include "cm_uart.h"
#include "cm_spi.h"

void blinkForever();
void blinkCommandLine();
void sMaster();
void sSlave();

int main(int argc, char** argv) {

    AD1PCFGL = 0xFFFF;

    TRISAbits.TRISA2 = 0;       // Set pin RA2 to be digital output
    PORTAbits.RA2 = 1;          // Set pin RA2 high

    // REFERENCE: Datasheet sec 11.1

    RPOR4bits.RP8R = 0b00011;   // Assign output U1TX to pin RP8
    RPINR18bits.U1RXR = 6;      // Assign input U1RX to pin RP6

    // REFERENCE: Datasheet sec 11.6.2.2 and 11.6.2.1 respectively,
    // This is using Peripheral Pin Select (PPS)

    configureUART1();           // Set up UART1 module w/ baud 9600

    //blinkCommandLine();

    RPOR0bits.RP0R = 0b00111;
    RPOR0bits.RP1R = 0b01000;
    RPOR1bits.RP2R = 0b01001;

    RPINR20bits.SDI1R = 3;
    RPINR20bits.SCK1R = 4;
    RPINR21bits.SS1R = 5;

    if (1) {
        configureSPI1Master();
        sMaster();
    } else {
        configureSPI1Slave();
        sSlave();
    }

    return (EXIT_SUCCESS);
}

void sMaster() {
    uart1_puts("\r\nBeginning master operation");

    int tx = 0;
    while (1) {
        tx++;
        //uprint_int("\r\nSending: ", tx);
        spi1Tx(tx);
    }

}

void sSlave() {
    uart1_puts("\r\nBeginning slave operation");
    while (1) {
        int value = spi1Rx();
        uprint_int("\r\nReceieved through SPI: ", value);
    }
}

void blinkForever() {
    long i = 0;
    while (1) {
        i = 300000;
        while (i--) {

        }
        PORTAbits.RA2 = ~PORTAbits.RA2;
        uart1_puts("Blink...\r\n");
    }
}

void blinkCommandLine() {

    char rx;
    char confirm_message[50];
    int count = 0;
    long i = 0;
    while (1) {
        // Set RA2 low
        PORTAbits.RA2 = 0;

        uart1_puts("\r\n\r\n");
        uart1_puts("Please enter a number between 1 and 9: ");

        rx = uart1Rx();

        uart1_puts("\r\n");

        int number = (int)rx - 48;

        sprintf(confirm_message, "Blinking %d times ... ", number);
        uart1_puts(confirm_message);

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
