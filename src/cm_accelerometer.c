#include <stdlib.h>
#include <xc.h>

#include "hardware.h"

#include "cm_uart.h"
#include "cm_accelerometer.h"

#define NUM_SAMPLES         512
#define NUM_SAMPLES_POWER   9       // 2^9 = 512

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

#define SPIN_COUNT_THRESHOLD    3

int trackingSpin;
int spinCounter;


void __attribute__ ((__interrupt__,no_auto_psv)) _T1Interrupt(void) {

    IFS0bits.T1IF = 0;

    if (trackingSpin) {
        if (nSamples < NUM_SAMPLES) {
            runningSum += readADCRaw();
            nSamples ++;
        }

        if (nSamples == NUM_SAMPLES) {

            int result = runningSum >> NUM_SAMPLES_POWER;
            nSamples = 0;
            runningSum = 0;

            if (result < 2500) {
                spinCounter = 0;
            } else {
                spinCounter ++;
            }

            if (spinCounter == SPIN_COUNT_THRESHOLD) trackingSpin = 0;

        }
    }

}

void startTrackingSpin() {
    trackingSpin = 1;
    spinCounter = 0;
    runningSum = 0;
    nSamples = 0;
}

int checkSpinComplete() {

    uprint_int("spinCounter: ", spinCounter);
    return (spinCounter == SPIN_COUNT_THRESHOLD);

}

int checkThrustComplete() {

}