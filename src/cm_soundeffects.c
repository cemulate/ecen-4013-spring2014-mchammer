#include "Common.h"
#include "hardware.h"

#include "cm_uart.h"
#include "cm_soundeffects.h"

void audioReset() {

    //hmmm
    pAUDIO_CLK_PORT = 1;
    pAUDIO_DATA_PORT = 1;

    pAUDIO_RESET_PORT = 1;
    //__delay_ms(5);    //5ms delay

    pAUDIO_RESET_PORT = 0;
    //__delay_ms(5);    //5ms delay

    pAUDIO_RESET_PORT = 1;

    DELAY_MS(500);
}

void playSound(int sound) {

    int i;
    pAUDIO_CLK_PORT = 1;
    pAUDIO_CLK_PORT = 0;
    DELAY_MS(2); // Wait 2ms before starting to send data

    for (i = 0; i < 16; i++) { // Loop to cycle through and send each bit
        pAUDIO_CLK_PORT = 0;
        pAUDIO_DATA_PORT = (sound >> (15 - i)) & 0x01; // Shift the andBit j times left and & it with the num input
        DELAY_US(200);
        pAUDIO_CLK_PORT = 1;
        DELAY_US(200);
    }
    pAUDIO_DATA_PORT = 1;

}

void setVolume(int vol) {

    // 1 to 8

    playSound(0xFFF0 + (vol - 1));

}
