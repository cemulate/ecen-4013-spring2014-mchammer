#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xc.h>

#include "cm_uart.h"

void configureUART1() {

    // *********** PERIPHERAL PIN SELECT ***********
    // Change this if needed

    RPOR4bits.RP8R = 0b00011;   // Assign output U1TX to pin RP8
    RPINR18bits.U1RXR = 6;      // Assign input U1RX to pin RP6
    
    // *********** PERIPHERAL PIN SELECT ***********


    U1MODE = 0x00;

    // U1BRG = (FCY / (16*baud)) - 1; baud = 9600

    U1BRG = 23;

    // Enable UART1 module, and enable transmission on it

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
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

    buffer[i] = 0;
}

char uart1Rx() {
    while (U1STAbits.URXDA == 0);
    return U1RXREG;
}

void uprint(char *message) {
    uart1_puts(message);
    uart1_puts("\r\n");
}

void uprint_int(char *message, int value) {

    int size = strlen(message);
    char buf[size + 10];
    sprintf(buf, "%s%d\r\n", message, value);
    uprint(buf);

}

void uprint_dec(char *message, double value) {

    int size = strlen(message);
    char buf[size + 10];
    sprintf(buf, "%s%.3f\r\n", message, value);
    uprint(buf);

}