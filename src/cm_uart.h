/* 
 * File:   cm_uart.h
 * Author: Chase Meadors
 *
 * Created on October 14, 2014, 1:24 AM
 */

#ifndef UART_H
#define	UART_H

void uart1Tx(char value);
char uart1Rx();

void uart1_puts(char *message);
void uart1_gets(char *buffer, int length);

void uprint(char *message);
void uprint_int(char *message, int value);
void uprint_dec(char *message, double value);

#endif	/* UART_H */

