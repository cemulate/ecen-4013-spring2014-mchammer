#include <stdlib.h>
#include <xc.h>

#include "HammerState.h"
#include "Common.h"
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

#define SPIN_ACC_THRESHOLD      2400
#define THRUST_ACC_THRESHOLD    2000

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
        // Collect a sample
        if (nSamples < NUM_SAMPLES) {
            runningSum += readADCRaw();
            nSamples ++;
        }

        // We've collected one group of samples
        if (nSamples == NUM_SAMPLES) {

            // Average the lsat NUM_SAMPLES measurements
            // This is implementing efficiently by using a right-shift
            // instead of a divide
            int result = runningSum >> NUM_SAMPLES_POWER;
            nSamples = 0;
            runningSum = 0;

            // If the last group of samples was "characterstic of
            // a spin", increment the spin counter. Otherwise, zero
            // it out to force the user to start over
            if (result < SPIN_ACC_THRESHOLD) {
                spinCounter = 0;
            } else {
                spinCounter ++;
            }

            // Stop tracking if we've completed it. As soon as this
            // condition is reached, checkSpinComplete() will return
            // true until startTrackingSpin() is called again
            if (spinCounter == SPIN_COUNT_THRESHOLD) trackingSpin = 0;

        }
    } else if (trackingThrust) {

        // Take one sample and compare it to the thrust threshold
        thrustComplete = (readADCRaw() > THRUST_ACC_THRESHOLD);

        // Stop trackng if we did it. At this point, checkThrustComplete()
        // will return true until startTrackingThrust() is called again
        if (thrustComplete) trackingThrust = 0;

    }
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
    thrustComplete = 0;
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