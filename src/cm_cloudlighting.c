#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "hardware.h"

#include "cm_uart.h"

#include "Common.h"

#include "cm_cloudlighting.h"

unsigned int display[16];

int lightmode = 0;

void cloudLightingSetMode(int algm) {
    lightmode = algm;
}


void cloudLightingSet(unsigned int num, unsigned int value) {
    display[num] = value;
}

void cloudLightingSetAll(unsigned int value) {
    int i = 0;
    for (i = 0; i < 16; i ++) {
        display[i] = value;
    }
}

unsigned int gscount = 0;

void cloudLightingUpdate() {

    IEC0bits.T2IE = 0;
    pCLIGHTS_BLANK_PORT = 1;

    int i = 0, j = 0;
    for (i = 0; i < 16; i ++) {
        for (j = 11; j >= 0; j --) {
            pCLIGHTS_SDO_PORT = (display[i] >> j) & 0x01;
            pCLIGHTS_SCK_PORT = 1;
            pCLIGHTS_SCK_PORT = 0;
        }
    }

    pCLIGHTS_XLAT_PORT = 1;
    pCLIGHTS_XLAT_PORT = 0;

    pCLIGHTS_BLANK_PORT = 0;
    TMR2 = 0;
    gscount = 0;
    IEC0bits.T2IE = 1;
}

// We have to wrap this with #ifdef cloud, because the cloud defines a
// T2Interrupt for its use, and even though we're not using it,
// the compiler would complain about re-definition
#ifdef CLOUD

void __attribute__ ((__interrupt__,no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;
    pCLIGHTS_BLANK_PORT = 1;
    pCLIGHTS_BLANK_PORT = 0;
}

#endif

unsigned int blink_timer = 0;        
unsigned int blink_timer_max = 30;   // Run at ~60Hz/30 = 2Hz
unsigned int blink_vals[4] = {4, 40, 400, 4000};
unsigned int blink_counter = 0;
void stepALGM_blink() {

    if (((blink_timer ++) % blink_timer_max) != 0) return;

    int i = 0;
    for (i = 0; i < 16; i ++) {
        cloudLightingSet(i, blink_vals[(i + blink_counter) % 4]);
    }

    blink_counter = (blink_counter + 1) % 4;

    cloudLightingUpdate();

}

// We have to wrap this with #ifdef cloud, because the accelerometer module
// Defines a T1Interrupt for the Hammer, and even though we're not using it,
// the compiler would complain about re-definition
#ifdef CLOUD

// This is called at around 60 Hz. Individual algorithms can define their
// own time steps to run at "lower resolution"
void __attribute__ ((__interrupt__,no_auto_psv)) _T1Interrupt(void) {

    IFS0bits.T1IF = 0;

    switch (lightmode) {
        case ALGM_OFF:
            cloudLightingSetAll(0);
            cloudLightingUpdate();
            break;
        case ALGM_BLINK:
            stepALGM_blink();
            break;
    }

}

#endif