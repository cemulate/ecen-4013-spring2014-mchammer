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

unsigned int nSamples;
unsigned long long int runningSum;

int getMA() {
    if (nSamples == 1024) {

        int result = runningSum >> 10;
        resetMotionHistory();
        return result;

    } else {
        return -1;
    }
}

void __attribute__ ((__interrupt__,no_auto_psv)) _T1Interrupt(void) {

    IFS0bits.T1IF = 0;

    if (nSamples < 1024) {
        runningSum += readADCRaw();
        nSamples ++;
    }

}

int checkSpinComplete() {



}

int checkThrustComplete() {

    

}

void resetMotionHistory() {
    nSamples = 0;
    runningSum = 0;
}