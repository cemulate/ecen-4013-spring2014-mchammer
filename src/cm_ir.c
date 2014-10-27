#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#include "cm_uart.h"
#include "cm_ir.h"
#include "HammerState.h"

#include "cm_delay.h"

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

int toggle;

void configureIRSend() {

    TRISBbits.TRISB5 = 0;
    PORTBbits.RB5 = 0;

    T2CONbits.TON = 0;          // Disable Timer
    T2CONbits.TCS = 0;          // Select internal instruction cycle clock
    T2CONbits.TGATE = 0;        // Disable Gated Timer mode
    T2CONbits.TCKPS = 0b01;     // Select 1:8 Prescaler
    TMR2 = 0x00;                // Clear timer register
    PR2 = 4;                    // Load the period value
    IPC1bits.T2IP = 0x01;       // Set Timer2 Interrupt Priority Level
    IFS0bits.T2IF = 0;          // Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1;          // Enable Timer2 interrupt
    T2CONbits.TON = 1;          // Start Timer

    toggle = 0;


}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {

    // Should be generated at ~114 kHz = (FCY / 8 / 4)

    IFS0bits.T2IF = 0;          // Clear Timer2 Interrupt Flag

    if (!toggle) {
        PORTBbits.RB5 = 1;
        toggle = 1;
    } else {
        PORTBbits.RB5 = 0;
        toggle = 0;
    }

}

void sendDamagePacket() {



//    //send damage packets
//    sendCycles(10);         //  send start packet
//    offCycles(140);
//    sendCycles(20);         //  send data packet
//    offCycles(130);
//    sendCycles(20);         //  send reduntant data packet
//    offCycles(130);
//    sendCycles(150);        //  send end packet
//
//    DELAY_US(2000);
}

void sendHealPacket() {
    //send damage packets
    sendCycles(10);         //  send start packet
    offCycles(140);
    sendCycles(30);         //  send data packet
    offCycles(120);
    sendCycles(30);         //  send reduntant data packet
    offCycles(120);
    sendCycles(150);        //  send end packet

    DELAY_US(2000);
}

void pulseLED()
{
    LED_PIN_PORT = 1;      // turn on IR
    DELAY_US(9);

    LED_PIN_PORT = 0;     // turn off IR
    DELAY_US(9);
}

void sendCycles(int n) {
    int i = n;
    while (i--) pulseLED();
}

void offCycles(int n) {
    int i = n;
    while (i--) DELAY_US(18);
}