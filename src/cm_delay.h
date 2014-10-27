/*
 * File:   cm_delay.h
 * Author: Chase
 *
 * Created on October 27, 2014, 4:58 AM
 */

#ifndef CM_DELAY_H
#define	CM_DELAY_H

#include <libpic30.h>

#define FOSC    (7370000ULL)
#define FCY     (FOSC/2)

#define CYCLES_PER_MS ((unsigned long)(FCY * 0.001))        //instruction cycles per millisecond
#define CYCLES_PER_US ((unsigned long)(FCY * 0.000001))   //instruction cycles per microsecond

#define DELAY_MS(ms)  __delay32(CYCLES_PER_MS * ((unsigned long) ms));   //__delay32 is provided by the compiler, delay some # of milliseconds
#define DELAY_US(us)  __delay32(CYCLES_PER_US * ((unsigned long) us));

#endif	/* CM_DELAY_H */

