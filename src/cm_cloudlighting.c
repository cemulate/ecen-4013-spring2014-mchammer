#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "hardware.h"

#include "cm_uart.h"

#include "Common.h"

unsigned char gsData[192] = {
// MSB LSB
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 15
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 14
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 13
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, // Channel 12
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, // Channel 11
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, // Channel 10
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, // Channel 9
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // Channel 8
0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, // Channel 7
0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, // Channel 6
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, // Channel 5
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 4
0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 3
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 2
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 1
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Channel 0
};

void cloudLED_spiSend(unsigned int data) {

	int i = 0;
    for(i = 0; i < 16; i++) {
        pCLIGHTS_SDO_PORT = (data >> (15-i));
        pCLIGHTS_SCK_PORT = 1;
        pCLIGHTS_SCK_PORT = 0;
    }

}


void sendCloudLEDState(unsigned int *p) {
    // Takes an array of 16 ints

    pCLIGHTS_XLAT_PORT = 0;

    int i = 0;
    for (i = 0; i < 16; i += 4) {
            unsigned int a = p[i];
            unsigned int b = p[i+1];
            unsigned int c = p[i+2];
            unsigned int d = p[i+3];
            cloudLED_spiSend((a << 4) | (b >> 8));
            cloudLED_spiSend((b << 8) | (c >> 4));
            cloudLED_spiSend((c << 12) | d);
    }

    pCLIGHTS_XLAT_PORT = 1;
    while (i--);
    pCLIGHTS_XLAT_PORT = 0;
}

void TLC5940_SetGS_And_GS_PWM() {
    char firstCycleFlag = 0;

    if (pCLIGHTS_VPRG_PORT) {
        pCLIGHTS_VPRG_PORT = 0;
        firstCycleFlag = 1;
    }

    unsigned int gsclk_count = 0;
    unsigned char data_count = 0;

    pCLIGHTS_BLANK_PORT = 0;
    while (1) {
        if (gsclk_count > 4095) {
            pCLIGHTS_BLANK_PORT = 1;
            pCLIGHTS_XLAT_PORT = 1;
            pCLIGHTS_XLAT_PORT = 0;
            if (firstCycleFlag) {
                pCLIGHTS_SCK_PORT = 1;
                pCLIGHTS_SCK_PORT = 0;
                firstCycleFlag = 0;
            }
            break;
        } else {
            if (!(data_count > (192 - 1))) {
                pCLIGHTS_SDO_PORT = gsData[data_count];
                pCLIGHTS_SCK_PORT = 1;
                pCLIGHTS_SCK_PORT = 0;
                data_count ++;
            }
        }
        pCLIGHTS_GSCLK_PORT = 1;
        pCLIGHTS_GSCLK_PORT = 0;
        gsclk_count ++;
    }
}