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
// Turn the watchdog off

// --------------------------------------------------------------

#include "HammerState.h"
#include "Common.h"
#include "hardware.h"
#include "radiohardware.h"

#include "cm_uart.h"
#include "cm_spi.h"
#include "cm_radio.h"
#include "cm_accelerometer.h"
#include "cm_soundeffects.h"
#include "cm_lightmcu.h"
#include "cm_ir.h"
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

    configureAccelerometer();

    configureAudio();
    audioReset();

    int sta = configureRadio(0x0A00, 0x0000111111111111);
    uprint_int("Configured radio: ", sta);

    configureIRReceive();

    configureLightMCU_SPI();

    updateLightMCUAll(hs->health, hs->charge);

    char sendString[2] = "x";
    char rxbuf[50];
    char doneString[] = "DONE";

    DELAY_MS(400);

    playSound(HS_BOOT);
    DELAY_MS(HS_BOOT_LEN);
    
    while (1) {

        uprint("Beginning of main loop");
        hs->charge = 0;
        updateLightMCUAll(hs->health, hs->charge);

        uprint("Waiting for spin...");
        startTrackingSpin();
        while (!checkSpinComplete());

        uprint("Spin complete!");
        playSound(HS_SPINCOMPLETE);
        DELAY_MS(HS_SPINCOMPLETE_LEN);

        setVolume(5);
        // Charging is really damn loud

        // This sound is very long, we just start it playing
        playSound(HS_CHARGING);

        uprint("Charging ...");

        hs->charge = 0;
        hs->charging = 1;
        while (hs->charge < 100) {
            hs->charge ++;
            updateLightMCUCharge(hs->charge);
            DELAY_MS((0.9 * hs->health) + 10);
        }
        hs->charging = 0;

        setVolume(8);
        // Back to normal

        playSound(HS_CHARGECOMPLETE);
        DELAY_MS(HS_CHARGECOMPLETE_LEN);

        uprint("Finished charging!");

        uprint("Waiting for thrust...");
        startTrackingThrust();
        while (!checkThrustComplete());

        setLightMCUHitAnim();

        uprint("Thrust complete!");
        playSound(HS_FIRE);
        DELAY_MS(HS_FIRE_LEN);

        setLightMCURainbow();

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

        enableIRReceive();
        uprint("Got cloud message");


        setLightMCUOff();
        DELAY_MS(30);

    }

}

void cloudMain() {

    uprint("\r\n ************************ BOOT UP (CLOUD) ************************ \r\n");

    configureAudio();
    audioReset();
    
    configureIRSend();
    int sta = configureRadio(0x0A00, 0x0000111111111111);
    uprint_int("Configured radio: ", sta);

    char rxbuf[2];
    int damageToSend, i;

    configureCloudLighting();

    DELAY_MS(400);

    playSound(CS_BOOT);
    DELAY_MS(CS_BOOT_LEN);


    while (1) {

        uprint("Beginning of main loop");
        cloudLightingSetMode(ALGM_BLINK);

        uprint("Waiting for hammer message");
        radioGetMessage(rxbuf, 1);

        uprint("Got message");

        playSound(CS_FIRE);
        // Don't wait for this sound

        // Enter manual lighting mode
        cloudLightingSetMode(ALGM_OFF);
        cloudLightingSetAll(BRIGHTNESS_MAX);
        cloudLightingUpdate();

        damageToSend = rxbuf[0];
        for (i = 0; i < damageToSend; i ++) {
            uprint("Sending damage packet");
            sendDamagePacket();
            if ((i % 10) == 0) {
                cloudLightingSetAll(BRIGHTNESS_MAX);
            } else if ((i % 5) == 0) {
                cloudLightingSetAll(50);
            }
            cloudLightingUpdate();
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
    char rx[50];
    unsigned char vol;
    unsigned int snd;
    while (1) {
        uprint("Enter volume level: ");
        uart1_gets(rx, 50);
        vol = atoi(rx);
        uprint_int("Volume is: ", vol);
        uprint("Enter sound: ");
        uart1_gets(rx, 50);
        snd = atoi(rx);
        uprint_int("Sound is: ", snd);
        setVolume(vol);
        playSound(snd);
    }
}

void TEST_LightMCU() {
    char rx[10];
    unsigned int i, r, g, b, final;
    while (1) {
        uprint("l to loop, other for special");
        rx[0] = uart1Rx();

        if (rx[0] == 'l') {
            for (i = 0; i < 192; i ++) {
                uprint_int("Sending ", i);
                sendLightMCU(i);
                DELAY_MS(50);
            }
        } else {
            uprint("Enter r 0-4: ");
            uart1_gets(rx, 10);
            r = atoi(rx);
            uprint("Enter g 0-4: ");
            uart1_gets(rx, 10);
            g = atoi(rx);
            uprint("Enter b 0-4: ");
            uart1_gets(rx, 10);
            b = atoi(rx);

            final = 0b11000000 + (r << 4) + (g << 2) + b;

            uprint_int("Sending ", final);

            sendLightMCU(final);
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