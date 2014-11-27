#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "hardware.h"

#include "cm_uart.h"
#include "cm_ir.h"
#include "cm_soundeffects.h"
#include "cm_accelerometer.h"
#include "cm_lightmcu.h"
#include "HammerState.h"

#include "Common.h"

#define IN_RANGE(x, a, b)       ((x >= a) && (x < b))

unsigned int c1 = 0, c2 = 0;
unsigned int diff;
int inPacket = 0;
int dCounter = 0, hCounter = 0;
int gotPulse = 0;
int rxState = 0;

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
    HammerState *hs = getHammerStatePtr();
    IFS0bits.IC1IF = 0; // Reset respective interrupt flag

    TMR2 = 0;
    while (pIRRX_PIN_PORT == 0);
    diff = TMR2;

    if ((diff > 1350) && (diff < 1750)) {
        dCounter ++;
        if (dCounter == 2) {
            hs->health = hs->health - 1;
            playSound(HAMMER_SOUND_DAMAGED);
            dCounter = 0;
        }
    } else if ((diff > 2000) && (diff < 2400)) {
        hCounter ++;
        if (hCounter == 2) {
            hs->health = hs->health + 1;
            playSound(HAMMER_SOUND_HEALED);
            hCounter = 0;
        }
    } else {
        dCounter = 0;
        hCounter = 0;
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

    sendCycles(10);
    offCycles(140);
    sendCycles(20);
    offCycles(130);
    sendCycles(20);
    offCycles(130);
    sendCycles(100);

    INT_ON();

    offCycles(250);

}

unsigned int pcnt = 0;
int irSendState;
int payloadLength = 20;
int sendComplete = 0;

//void sendDamagePacket() {
//    INT_OFF();
//    pcnt = 0;
//    irSendState = 1;
//    payloadLength = 20;
//    sendComplete = 0;
//    IEC1bits.T4IE = 1;
//
//    while (!sendComplete);
//
//    INT_ON();
//    IEC1bits.T4IE = 0;
//
//}
//
//#define SENDMASK(p)     (IN_RANGE(p, 0, 10) ||                      \
//                         IN_RANGE(p, 150, 150+payloadLength) ||     \
//                         IN_RANGE(p, 300, 300+payloadLength) ||     \
//                         IN_RANGE(p, 450, 600))                     \
//
//void __attribute__ ((__interrupt__,no_auto_psv)) _T4Interrupt(void) {
//    IFS1bits.T4IF = 0;
//    pLED_PIN_PORT = SENDMASK(pcnt) ? irSendState : 0;
//    irSendState = !irSendState;
//    if (irSendState) pcnt ++;
//    if (pcnt == 600) sendComplete = 1;
//}

void sendHealPacket() {
    
}

// DON'T FUCKING TOUCH THIS SHIT
// UNLESS YOU WANT IR TO BREAK
#define PHALF   22
#define PFULL   50

void pulseLED()
{
    pLED_PIN_PORT = 1;      // turn on IR
    __delay32(PHALF);

    pLED_PIN_PORT = 0;     // turn off IR
    __delay32(PHALF);
}

void sendCycles(int n) {
    int i = n;
    while (i--) pulseLED();
}

void offCycles(int n) {
    int i = n;
    while (i--) __delay32(PFULL);
}