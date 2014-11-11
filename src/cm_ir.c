#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "hardware.h"

#include "cm_uart.h"
#include "cm_ir.h"
#include "cm_soundeffects.h"
#include "HammerState.h"

#include "Common.h"

#define IN_RANGE(x, a, b)       ((x >= a) && (x <= b))

unsigned int c1, c2;
unsigned int diff;
int inPacket;
int dCounter, hCounter;

// Private functions
void sendCycles(int n);
void offCycles(int n);

void initIRReceive() {
    inPacket = 0;
    dCounter = 0;
    hCounter = 0;
}

void __attribute__ ((__interrupt__,no_auto_psv)) _IC1Interrupt(void)
{

    IFS0bits.IC1IF = 0; // Reset respective interrupt flag

    if (!inPacket) {
        c1 = IC1BUF;
        inPacket = 1;
        disableMotionTracking();
    } else {
        c2 = IC1BUF;
        inPacket = 0;
        enableMotionTracking();
    }

    if (!inPacket) {
        diff = c2 - c1;
        //uprint_int("diff: ", diff);
        if ((diff > 1350) && (diff < 1750)) {
            dCounter ++;
        } else if ((diff > 2000) && (diff < 2400)) {
            hCounter ++;
        }
    }

    HammerState *hs = getHammerStatePtr();

    if (dCounter == 2) {
        dCounter = 0;
        hs->health = hs->health - 1;
        setLightMCU_Red();
    }

    if (hCounter == 2) {
        hCounter = 0;
        hs->health = hs->health + 1;
        setLightMCU_Green();
    }
}

void disableIRReceive() {
    IEC0bits.IC1IE = 0;
}

void enableIRReceive() {
    IEC0bits.IC1IE = 1;
}

// **********************************************************************


#define TQ_LENGTH       150
#define TQ_START_ON     10
#define TQ_START_OFF    140
#define TQ_DAMAGE_ON    20
#define TQ_DAMAGE_OFF   130
#define TQ_HEAL_ON      30
#define TQ_HEAL_OFF     120

int sendCount;

void sendDamagePacket() {

    INT_OFF();

    sendCycles(TQ_START_ON);
    offCycles(TQ_START_OFF);
    sendCycles(TQ_DAMAGE_ON);
    offCycles(TQ_DAMAGE_OFF);
    sendCycles(TQ_DAMAGE_ON);
    offCycles(TQ_DAMAGE_OFF);
    sendCycles(TQ_LENGTH);

    INT_ON();

    offCycles(TQ_LENGTH*2);

}

void sendHealPacket() {
    
}

#define PHALF   7.7         // Tuned to produce 56kHz
#define PFULL   15.4

void pulseLED()
{
    pLED_PIN_PORT = 1;      // turn on IR
    DELAY_US(PHALF);

    pLED_PIN_PORT = 0;     // turn off IR
    DELAY_US(PHALF);
}

void sendCycles(int n) {
    int i = n;
    while (i--) pulseLED();
}

void offCycles(int n) {
    int i = n;
    while (i--) DELAY_US(PFULL);
}