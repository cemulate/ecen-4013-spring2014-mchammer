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
#include "hardware.h"

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

#pragma config FWDTEN = OFF
// Turn that watchdog shit the fuck off

// --------------------------------------------------------------

#include "cm_uart.h"
#include "cm_spi.h"

#include "radiohardware.h"
#include "MRF24J40.h"
#include "cm_radio.h"

#include "cm_accelerometer.h"
#include "cm_soundeffects.h"

#include "cm_lightmcu.h"

#include "cm_ir.h"

#include "Common.h"

#include "HammerState.h"

#include "cm_cloudlighting.h"

void blinkForever();
void blinkCommandLine();

void radioSenderDemo();
void radioReceiverDemo();

void hammerMain();
void cloudMain();

#define WHO     1

int main(int argc, char** argv) {

    AD1PCFGL = 0xFFFF;          // Analog? Hell naw.
    configureUART1();           // Set up UART1 module w/ baud 9600

#ifdef HAMMER
    hammerMain();
#elif CLOUD
    cloudMain();
#endif

    return (EXIT_SUCCESS);
}

void hammerMain() {

    uprint("\r\n ######################## BOOT UP (HAMMER) ######################## \r\n");

    initHammerState();
    HammerState *hs = getHammerStatePtr();

    //configureAccelerometer();
    configureAudio();
    int sta = configureRadio(0x0A00, 0x0000111111111111);
    uprint_int("Configured radio: ", sta);

    configureIRReceive();

    //configureLightMCU_SPI();

    char sendString[2] = "x";
    char rxbuf[50];
    char doneString[] = "DONE";

    playSound(HAMMER_SOUND_BOOT);

    while (1) {
        uprint_int("Hammer health: ", hs->health);
    }

    while (1) {

        uprint("Beginning of main loop");

        uprint("Waiting for spin...");
        startTrackingSpin();
        while (!checkSpinComplete());

        uprint("Spin complete!");
        playSound(HAMMER_SOUND_SPINCOMPLETE);

        DELAY_MS(3000);

        playSound(HAMMER_SOUND_CHARGING);

        uprint("Charging ...");

        hs->chargeStatus = 0;
        hs->charging = 1;
        while (hs->chargeStatus < 100) {
            hs->chargeStatus ++;
            DELAY_MS(20);
        }
        hs->charging = 0;

        // No sounds for now
        uprint("Finished charging!");

        playSound(HAMMER_SOUND_SPINCOMPLETE);

        uprint("Waiting for thrust...");
        startTrackingThrust();
        while (!checkThrustComplete());

        uprint("Thrust complete!");
        playSound(HAMMER_SOUND_FIRE);

        // Become invincible
        disableIRReceive();

        uprint("Sending radio message");
        sendString[0] = hs->health;
        radioSendMessage(sendString, 0x0A00);

        uprint("Waiting for cloud message");
        radioGetMessage(rxbuf, 50);

        if (memcmp(rxbuf, doneString, 4) != 0) {
            uprint("Invalid message from cloud!");
        }

        uprint("Got clousd message");

        enableIRReceive();

    }

}

void cloudMain() {

    uprint("\r\n ************************ BOOT UP (CLOUD) ************************ \r\n");

    configureAudio();
    configureIRSend();
    int sta = configureRadio(0x0A00, 0x0000111111111111);
    uprint_int("Configured radio: ", sta);

    char rxbuf[2];
    int damageToSend, i;

    configureCloudLighting();
    cloudLightingSetMode(ALGM_BLINK);

    playSound(CLOUD_SOUND_BOOT);

    while(1) {
        sendDamagePacket();
        DELAY_MS(10);
    }

    while (1) {

        uprint("Beginning of main loop");

        uprint("Waiting for hammer message");
        radioGetMessage(rxbuf, 1);

        uprint("Got message");

        playSound(CLOUD_SOUND_FIRE);

        damageToSend = rxbuf[0];
        for (i = 0; i < damageToSend; i ++) {
            uprint("Sending damage packet");
            sendDamagePacket();
        }

        uprint("Sending DONE message");
        radioSendMessage("DONE", 0x0A00);

    }
    
}

void TEST_RadioSend() {
    char rx[2];
    while (1) {
        uprint("Enter character to send: ");
        uart1_gets(rx, 1);
        radioSendMessage(rx, 0x0A00);
        uprint("Sent message!");
    }
}

void TEST_RadioReceive() {
    char rx[2];
    while (1) {
        uprint("Wating for message ...");

        radioGetMessage(rx, 1);

        uprint("Got message: ");
        uprint(rx);
    }
}

void TEST_Audio() {
    configureAudio();

    char tx;
    while(1) {
        uprint("Enter number of sound to play: ");
        tx = uart1Rx() - 48;
        playSound((int)tx);
    }
}

void TEST_LightMCU() {
    configureLightMCU_SPI();

    int i = 0;
    while (1) {
        for (i = 1; i <= 100; i ++) {
            uprint_int("Sending ", i);
            sendLightMCU(i + 100);
            DELAY_MS(50);
        }
    }
}

void TEST_CloudLighting() {
    configureCloudLighting();
    cloudLightingSetMode(ALGM_BLINK);
    uprint("Should be blinking...");
}

void TEST_IRReceive() {
    configureIRReceive();
    
}

void TEST_IRSend() {
    while (1) {
        sendDamagePacket();
    }
}