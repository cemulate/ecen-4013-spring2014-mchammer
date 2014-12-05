#include "Common.h"
#include "hardware.h"

#include "cm_uart.h"

#include <string.h>

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
    while (i < length) {

        buffer[i] = uart1Rx();
        if (buffer[i] == '\r') {
            buffer[i] = 0;
            return;
        }

        i ++;

    }
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

void uprint_uint(char *message, unsigned int value) {

    int size = strlen(message);
    char buf[size + 10];
    sprintf(buf, "%s%u\r\n", message, value);
    uprint(buf);

}

void uprint_dec(char *message, double value) {

    int size = strlen(message);
    char buf[size + 10];
    sprintf(buf, "%s%.3f\r\n", message, value);
    uprint(buf);

}