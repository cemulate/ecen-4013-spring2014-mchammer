#include <stdlib.h>
#include <math.h>

#include <xc.h>
#include "hardware.h"
#include "Common.h"

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

void updateLightMCU(int health, int charge) {

    sendLightMCU(0xFF);
    sendLightMCU(health);
    sendLightMCU(0xEE);
    sendLightMCU(charge);

}
