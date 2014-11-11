#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "hardware.h"

#include "cm_uart.h"

#include "Common.h"



void cloudLightingOn() {
    pCLIGHTS_PIN_PORT = 1;
}

void cloudLightingOff() {
    pCLIGHTS_PIN_PORT = 0;
}