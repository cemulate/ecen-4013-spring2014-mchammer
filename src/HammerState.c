#include <stdlib.h>
#include <xc.h>

#include "cm_uart.h"
#include "HammerState.h"

static volatile HammerState hState;

void initHammerState() {
    hState.health = 100;
    hState.charge = 0;
}

HammerState * getHammerStatePtr() {
    return (HammerState *)&hState;
}