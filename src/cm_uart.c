#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xc.h>

#include "cm_uart.h"

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

void uart1_puts(char *message) {
    int size = strlen(message);
    int i = 0;

    for (i = 0; i < size; i ++) {

        uart1Tx(message[i]);

    }
}

void uart1Tx(char value) {
    while (U1STAbits.UTXBF == 1);
    U1TXREG = value;
}

void uart1_gets(char *buffer, int length) {
    int i = 0;
    for (i = 0; i < length; i ++) {

        buffer[i] = uart1Rx();

    }

    // Make the contents of buffer a valid C string by ending
    // it with the null character

    buffer[i+1] = '\0';
}

char uart1Rx() {
    while (U1STAbits.URXDA == 0);
    return U1RXREG;
}

void uprint_int(char *message, int value) {

    int size = strlen(message);
    char buf[size + 10];
    sprintf(buf, "\r\n%s%d", message, value);
    uart1_puts(buf);

}

void uprint_dec(char *message, double value) {

    int size = strlen(message);
    char buf[size + 10];
    sprintf(buf, "\r\n%s%.3f", message, value);
    uart1_puts(buf);

}