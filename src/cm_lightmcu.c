#include <stdlib.h>
#include <math.h>

#include <xc.h>
#include "hardware.h"
#include "Common.h"
#include "HammerState.h"

#include "cm_lightmcu.h"
#include "cm_uart.h"

void sendLightMCU(unsigned char data) {
    pLIGHTMCU_SCK_PORT = 0;

    int i;
    for(i = 0; i < 8; i++) {
        pLIGHTMCU_SDO_PORT = (data >> (7-i));
        pLIGHTMCU_SCK_PORT = 1;
        pLIGHTMCU_SCK_PORT = 0;
    }
}


// health in [0, 100)
void updateLightMCUHealth(unsigned int health) {
    if (health > 95) health = 95;
    sendLightMCU(health);
}

// charge in [0, 100)
void updateLightMCUCharge(unsigned int charge) {
    if (charge < 5) charge = 5;
    sendLightMCU(96 + charge - 5);
}

void updateLightMCUAll(unsigned int health, unsigned int charge) {
    updateLightMCUHealth(health);
    DELAY_MS(30);
    updateLightMCUCharge(charge);
}

// r, g, b are 0 or 1
void setLightMCUColor(unsigned int r, unsigned int g, unsigned int b) {
    unsigned int send = 0b11000000 + (r << 4) + (g << 2) + b;
    sendLightMCU(send);
}

void setLightMCURainbow() {
    sendLightMCU(0xFF);
}

void setLightMCUOff() {
    sendLightMCU(0b11000000);
}


void disableLightMCUUpdates() {
    IEC0bits.T3IE = 0;
}

void enableLightMCUUpdates() {
    IEC0bits.T3IE = 1;
}

int timeout = 60;
int timeout_active = 0;

void requestLightMCUUpdateTimeout() {
    timeout = 60;
    timeout_active = 1;
}

// We have to wrap this with #ifdef cloud, because the cloud defines a
// T2Interrupt for its use, and even though we're not using it,
// the compiler would complain about re-definition
#ifdef HAMMER

int healthCycle = 1;

// This is called at around 60 Hz.
void __attribute__ ((__interrupt__,no_auto_psv)) _T3Interrupt(void) {

    IFS0bits.T3IF = 0;

//    if (!timeout_active) {
//        if (healthCycle) {
//            updateLightMCUHealth(getHammerStatePtr()->health);
//        } else {
//            updateLightMCUCharge(getHammerStatePtr()->chargeStatus);
//        }
//        healthCycle = !healthCycle;
//    } else {
//        timeout --;
//        if (timeout == 0) timeout_active = 0;
//    }
    
}

#endif