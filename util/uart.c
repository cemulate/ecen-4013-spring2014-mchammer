#include <stdlib.h>
#include <string.h>

#include <p24HJ64GP502.h>

#include "uart.h"

void configureUART1() {

    // Zeroing out U1MODE has the effect of giving us pretty much all
    // the behavior we want, including 8-bit data, 1 stop bit, and no parity

    U1MODE = 0x00;

    // U1BRG = (FCY / (16*baud)) - 1; baud = 9600

    U1BRG = 23;

    // Enable UART1 module, and enable transmission on it

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    // REFERENCE:
    // dsPIC33/PIC24 Family Reference Manual - Chapter 17
    // PIC24HJ64GP502 Datasheet, section 18
}

void uart1Tx(char *message) {

    int size = strlen(message);
    int i = 0;

    for (i = 0; i < size; i ++) {

        // Loop through each byte in message;
        // Wait while the tx buffer is still full,
        // and when it becomes empty, put another
        // character in the transmit register

        while (U1STAbits.UTXBF == 1);
        U1TXREG = message[i];

    }

    // REFERENCE:
    // PIC24HJ64GP502 Datasheet, section 18
}

void uart1Rx(char *buffer, int length) {

    int i = 0;
    for (i = 0; i < length; i ++) {

        // Wait until there is data in the rx buffer
        // then write it to the buffer

        while (U1STAbits.URXDA == 0);
        buffer[i] = U1RXREG;

    }

    // Make the contents of buffer a valid C string by ending
    // it with the null character

    buffer[i+1] = '\0';

    // REFERENCE:
    // PIC24HJ64GP502 Datasheet, section 18
}
