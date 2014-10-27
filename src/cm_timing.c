#include <stdlib.h>
#include <xc.h>

#include "cm_timing.h"

#include "HammerState.h"

#include "MRF24J40.h"

#define DELTA_T (1.0 / 1600.0)

int countMod2;

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {

    // Should be generated at 1600 Hz = (FCY / 256 / 9)

    // Do all stuff that happens on an interval!
    
    HammerState_doCharge(DELTA_T);

    IFS0bits.T1IF = 0;  // Clear Timer1 Interrupt Flag
}

void configureTimer1() {

    // Configure Timer1 to generate an interrupt at 1600 Hz

    T1CONbits.TON = 0;          // Disable Timer
    T1CONbits.TCS = 0;          // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;        // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11;     // Select 1:256 Prescaler
    TMR1 = 0x00;                // Clear timer register
    PR1 = 9;                    // Load the period value
    IPC0bits.T1IP = 0x01;       // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;          // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;          // Enable Timer1 interrupt
    T1CONbits.TON = 1;          // Start Timer

    countMod2 = 0;
}
