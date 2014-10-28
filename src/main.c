/* 
 * File:   main.c
 * Author: Chase
 *
 * Created on October 13, 2014, 3:46 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <xc.h>

// --------------------- Configuration bits ---------------------

// Configure clock settings:

#pragma config FNOSC = FRC
// Oscillator Mode (Internal Fast RC (FRC) with divide by N)
// This is the default clock. The FRC oscillates at a nominal 7.37 MHz,
// And "N" is by default 1, leaving our processor clocked at 7.37 MHz
// The instruction frequency is Fosc/2 = 3.685 MHz

#pragma config IESO = OFF
// Internal External Switch Over Mode = OFF
// Tells the processor to exclusively use the oscillator chosen above
// when starting up

// For previous two, see PIC24HJ64GP502 Datasheet sec. 9

#pragma config ALTI2C = OFF
// Tells the PIC to map I2C pins to ASDA1 and ASCL1, as opposed to
// the originals - SDA1 and SCL1. Datasheet sec. 25.1

#pragma config JTAGEN = OFF
// Disable JTAG (we don't need it = less pins taken up). Datasheet sec. 25.1

#pragma config OSCIOFNC = ON
// Tell the OSC2 pin to behave as a regular digital I/O pin, instead of
// exporting the internal clock signal. Datasheet sec 25.1

// --------------------------------------------------------------

#include "cm_uart.h"
#include "cm_spi.h"
#include "cm_adc.h"

#include "radiohardware.h"
#include "MRF24J40.h"
#include "cm_radio.h"

#include "cm_accelerometer.h"
#include "cm_soundeffects.h"

#include "cm_timing.h"

#include "cm_lightmcu.h"

#include "cm_ir.h"

#include "Common.h"

#include "HammerState.h"

void blinkForever();
void blinkCommandLine();

void radioSenderDemo();
void radioReceiverDemo();

int main(int argc, char** argv) {

    AD1PCFGL = 0xFFFF;          // Analog? Hell naw.

    // Set up peripherals, devices, and state
    
    configureUART1();           // Set up UART1 module w/ baud 9600

    uprint("\r\n ************************ BOOT UP ************************ \r\n");

    initHammerState();
    HammerState *gHammerState = getHammerStatePtr();

    configureTimer1();
    
    configureRadio(0x0A00, 0x0000111111111111);

    configureLightMCU_SPI();

    configureADC(9);
    
    //configureIRReceive();
    configureIRSend();

    char out[50];
    while (1) {
        sendDamagePacket();
    }

    // This will continually print out health - which should be
    // automatically responsive to IR damage/healing, if
    // configureIRReceive() was called
    while (1) {
        uprint_dec("Health: ", gHammerState->health);
    }

    char doneString[50] = "DONE";
    char rxbuf[50] = "DONE";

    while (1) {

        while (!checkSpinComplete());
        resetMotionHistory();

        playSound(SOUND_SPIN_COMPLETE);

        gHammerState->chargeRate = 100 * exp(-0.023 * gHammerState->health);
        gHammerState->charging = 1;

        while (gHammerState->chargeStatus < 100) {
            playSound(SOUND_CHARGING);
            sendLightStateUpdate(gHammerState->health, gHammerState->chargeStatus);
        }

        playSound(SOUND_CHARGING_COMPLETE);

        gHammerState->charging = 0;
        gHammerState->chargeStatus = 0;

        uprint_dec("Hammer charge status: ", gHammerState->chargeStatus);

        while (!checkThrustComplete());
        resetMotionHistory();

        // radioSendMessage("FIRE", 0x0A00);

        gHammerState->invincible = 1;

        // radioGetMessage(rxbuf, 50);
        uprint("Press key when cloud sends packet back... ");
        uart1Rx();

        if (memcmp(rxbuf, doneString, 4) != 0) {
            uprint("Error, invalid packet from cloud!");
            return 1;
        }

        gHammerState->invincible = 0;

    }

    return (EXIT_SUCCESS);
}

void setupLED() {
    TRISAbits.TRISA2 = 0;       // Set pin RA2 to be digital output
    PORTAbits.RA2 = 1;          // Set pin RA2 high
}

void blinkOnce() {
    PORTAbits.RA2 = 1;
    long i = 10000;
    while (i--);
    PORTAbits.RA2 = 0;
    i = 10000;
    while (i--);
}

void radioSenderDemo() {
    char rx[2];
    while (1) {
        uprint("Enter character to send: ");
        uart1_gets(rx, 1);
        radioSendMessage(rx, 0x0A00);
        uprint("Sent message!");
    }
}

void radioReceiverDemo() {
    char rx[2];
    while (1) {
        uprint("Wating for message ...");

        radioGetMessage(rx, 1);

        uprint("Got message: ");
        uprint(rx);

        blinkOnce();
    }
}

void blinkForever() {
    long i = 0;
    while (1) {
        i = 300000;
        while (i--) {

        }
        PORTAbits.RA2 = ~PORTAbits.RA2;
        uprint("Blink...");
    }
}

void blinkCommandLine() {

    char rx;
    char confirm_message[50];
    int count = 0;
    long i = 0;
    while (1) {
        // Set RA2 low
        PORTAbits.RA2 = 0;

        uprint("\r\n");
        uprint("Please enter a number between 1 and 9: ");

        rx = uart1Rx();

        int number = (int)rx - 48;

        sprintf(confirm_message, "Blinking %d times ... ", number);
        uprint(confirm_message);

        for (count = 0; count < number; count ++) {

            for (i = 0; i < 30000; i ++) {

            }

            PORTAbits.RA2 = ~PORTAbits.RA2;

            for (i = 0; i < 30000; i ++) {

            }

            PORTAbits.RA2 = ~PORTAbits.RA2;

        }
    }
}
