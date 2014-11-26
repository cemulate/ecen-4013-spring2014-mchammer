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

void setLightMCU_Red() {
    sendLightMCU(32);
}

void setLightMCU_Green() {
    sendLightMCU(100);
}

int currentAnimation = ANIM_NONE;

void requestAnimation(int anim) {
    currentAnimation = anim;
}

//unsigned int flashtwice_counter = 0;
//unsigned int flashtwice_max = 15;       // Run at ~60/15 = 4 Hz
//unsigned int flashtwice_stage = 0;
//void stepANIM_flashtwice() {
//
//    if (flashtwice_stage % 2 == 0) {
//
//    }
//
//}

// We have to wrap this with #ifdef cloud, because the cloud defines a
// T2Interrupt for its use, and even though we're not using it,
// the compiler would complain about re-definition
#ifdef HAMMER

// This is called at around 60 Hz.
void __attribute__ ((__interrupt__,no_auto_psv)) _T2Interrupt(void) {

    IFS0bits.T2IF = 0;

    // If we're not doing anything else, display health/charge

    switch (currentAnimation) {
        case ANIM_NONE:
            sendLightMCU(getHammerStatePtr()->health);
            break;
        case ANIM_FLASHTWICE:
            break;
    }
    
}

#endif