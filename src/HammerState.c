#include "HammerState.h"
#include "Common.h"

#include "cm_uart.h"
#include "cm_lightmcu.h"
#include "cm_soundeffects.h"

static volatile HammerState hState;

void initHammerState() {
    hState.health = 100;
    hState.charge = 0;
    hState.charging = 0;
}

void healthUp() {
    hState.health = (hState.health < 100) ? hState.health + 1 : 100;
}

void healthDown() {
    hState.health --;
    if (hState.health == 0) permaDeath();
}

void permaDeath() {
    DELAY_MS(50);
    setLightMCUColor(1, 0, 0);
    playSound(HS_DIE);

    // This is where I would send my CAN message ...
    // IF I HAD ONE!

    while (1);
}

void stun() {
    DELAY_MS(50);
    setLightMCUColor(0, 0, 1);
    if (!hState.charging) playSound(HS_STUNNED);
    DELAY_MS(3000);
    updateLightMCUAll(hState.health, hState.charge);
    DELAY_MS(50);
}

HammerState * getHammerStatePtr() {
    return (HammerState *)&hState;
}