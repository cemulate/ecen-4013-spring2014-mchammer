/* 
 * File:   cm_spi.h
 * Author: Chase Meadors
 *
 * Created on October 16, 2014, 6:48 PM
 */

#ifndef CM_SPI_H
#define	CM_SPI_H

void configureSPI1Master();
void configureSPI1Slave();

void spi1Tx(int data);
int spi1Rx();

#endif	/* CM_SPI_H */

