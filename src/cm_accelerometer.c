#include <stdlib.h>
#include <xc.h>

#include "hardware.h"

#include "cm_uart.h"
#include "cm_accelerometer.h"
#include "cm_lightmcu.h"
#include "HammerState.h"

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

int trackingThrust;
int thrustComplete;

// We have to wrap this with #ifdef HAMMER, because the cloud
// Defines a T1Interrupt for its use, and even though we're not using it,
// the compiler would complain about re-definition
#ifdef HAMMER

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
    } else if (trackingThrust) {

        thrustComplete = (readADCRaw() < 1500);
        if (thrustComplete) trackingThrust = 0;

    }

    if (!getHammerStatePtr()->charging) sendLightMCU(getHammerStatePtr()->health);
}

#endif

void startTrackingSpin() {
    trackingSpin = 1;
    trackingThrust = 0;
    spinCounter = 0;
    runningSum = 0;
    nSamples = 0;
}

int checkSpinComplete() {

    return (spinCounter == SPIN_COUNT_THRESHOLD);

}

void startTrackingThrust() {
    trackingSpin = 0;
    trackingThrust = 1;
}

int checkThrustComplete() {
    return thrustComplete;
}

void disableMotionTracking() {
    IEC0bits.T1IE = 0;
}

void enableMotionTracking() {
    IEC0bits.T1IE = 1;
}