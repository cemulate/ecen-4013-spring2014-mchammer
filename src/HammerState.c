#include <stdlib.h>
#include <xc.h>

#include "cm_uart.h"
#include "HammerState.h"

static volatile HammerState hState;

void initHammerState() {
    hState.health = 100;
    hState.chargeStatus = 0.0;
    hState.chargeRate = 20.0;        // Charge / sec
    hState.charging = 0;
}

HammerState * getHammerStatePtr() {
    return (HammerState *)&hState;
}

void HammerState_doCharge(double dt) {
    hState.chargeStatus += dt * hState.chargeRate;
}