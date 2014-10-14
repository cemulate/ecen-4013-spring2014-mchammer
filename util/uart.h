/* 
 * File:   uart.h
 * Author: Chase
 *
 * Created on October 14, 2014, 1:24 AM
 */

#ifndef UART_H
#define	UART_H

void configureUART1();

void uart1Tx(char *message);
void uart1Rx(char *buffer, int length);

#endif	/* UART_H */

