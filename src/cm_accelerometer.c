#include <stdlib.h>
#include <xc.h>

#include "hardware.h"

#include "cm_uart.h"
#include "cm_accelerometer.h"

unsigned int readADCRaw() {
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

double readADCPercent() {
    return 100 * (readADCRaw() / 4096.0);
}

int checkSpinComplete() {
    
    uprint("Press key when spin is complete: ");
    uart1Rx();

    return 1;

}

int checkThrustComplete() {

    uprint("Press key when thrust complete: ");
    uart1Rx();

    return 1;

}

void resetMotionHistory() {
    
}