#include <stdlib.h>
#include <xc.h>

#include "cm_uart.h"
#include "cm_soundeffects.h"

int playSound(SoundEffect s) {

    // Nonblocking

    uart1_puts("Playing some sound... ");

    return 1;

}
