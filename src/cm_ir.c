#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#include "cm_uart.h"
#include "cm_ir.h"
#include "HammerState.h"

#include "Common.h"

#define IN_RANGE(x, a, b)       ((x >= a) && (x <= b))

unsigned int c1, c2;
unsigned int diff;
int inPacket;
int dCounter, hCounter;

#define LED_PIN_TRIS    TRISBbits.TRISB5
#define LED_PIN_PORT    PORTBbits.RB5

// Private functions
void sendCycles(int n);
void offCycles(int n);

void configureIRReceive() {

    T2CON = 0x8000;             // 1:1 prescale, Timer 2 on

    TRISBbits.TRISB4 = 1;       // Set pin RB4 as input
    RPINR7bits.IC1R = 4;        // Set input IC1 to pin RP4

    IC1CONbits.ICM = 0b00;      // Disable Input Capture 1 module
    IC1CONbits.ICTMR = 1;       // Select Timer2 as the IC1 Time base
    IC1CONbits.ICI = 0b01;      // Interrupt on every second capture event
    IC1CONbits.ICM = 0b001;     // Re-enable / Generate capture event every edge

    // Enable Capture Interrupt And Timer2
    IPC0bits.IC1IP = 1;         // Setup IC1 interrupt priority level
    IFS0bits.IC1IF = 0;         // Clear IC1 Interrupt Status Flag
    IEC0bits.IC1IE = 1;         // Enable IC1 interrupt

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
    } else {
        c2 = IC1BUF;
        inPacket = 0;
    }

    if (!inPacket) {
        diff = c2 - c1;
        if ((diff > 1350) && (diff < 1750)) {
            dCounter ++;
        } else if ((diff > 2000) && (diff < 2400)) {
            hCounter ++;
        }
    }

    if (dCounter == 2) {
        dCounter = 0;
        getHammerStatePtr()->health = getHammerStatePtr()->health - 1;
    }

    if (hCounter == 2) {
        hCounter = 0;
        getHammerStatePtr()->health = getHammerStatePtr()->health + 1;
    }
}

// **********************************************************************


#define TQ_START    10
#define TQ_DAMAGE   20
#define TQ_HEAL     30
#define TQ_STOP     150


int sendCount;

int toggle, val, sent;

void configureIRSend() {

    TRISBbits.TRISB5 = 0;
    PORTBbits.RB5 = 0;

//    RPOR2bits.RP5R = 0b10010;   // Output OC1 to pin RP5

//    OC1CON = 0;                 // It is a good practice to clear off the control bits initially
//    OC1CONbits.OCTSEL = 0;      // Timer2 is source
//
//    OC1R = 33;
//    OC1RS = 66;

    T2CON = 0;
    T2CONbits.TCKPS = 0b00;
    T2CONbits.TCS = 0;
    PR2 = 33;
    T2CONbits.TON = 1;

//    OC1CONbits.OCM = 0b000;         // Off for now

//    IPC0bits.OC1IP = 1;         // Setup OC1 interrupt priority level
//    IFS0bits.OC1IF = 0;         // Clear OC1 Interrupt Status Flag
//    IEC0bits.OC1IE = 1;         // Enable OC1 interrupt

//    IPC1bits.T2IP = 1;
//    IFS0bits.T2IF = 0;
//    IEC0bits.T2IE = 0;

    sendCount = 0;

    toggle = 0;

}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {

    IFS0bits.T2IF = 0;

    if (!toggle) {
        PORTBbits.RB5 = val;
        toggle = 1;
    } else {
        PORTBbits.RB5 = 0;
        toggle = 0;
        sendCount ++;
    }

}

void sendDamagePacket() {

    INT_OFF();

    sendCycles(10);
    offCycles(140);
    sendCycles(20);
    offCycles(130);
    sendCycles(20);
    offCycles(130);
    sendCycles(150);
    offCycles(500);

    INT_ON();

}

void sendHealPacket() {
    
}

void pulseLED()
{
    LED_PIN_PORT = 1;      // turn on IR
    DELAY_US(8.82);

    LED_PIN_PORT = 0;     // turn off IR
    DELAY_US(8.82);
}

void sendCycles(int n) {
    int i = n;
    while (i--) pulseLED();
}

void offCycles(int n) {
    int i = n;
    while (i--) DELAY_US(8.82*2);
}