#include <stdlib.h>
#include <xc.h>

#include "cm_uart.h"
#include "HammerState.h"

static HammerState hState;

void HammerState_Init() {
    hState.health = 0.0;
    hState.chargeStatus = 0.0;
}