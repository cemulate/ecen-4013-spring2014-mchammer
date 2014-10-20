#include <stdlib.h>
#include <xc.h>

#include "cm_uart.h"
#include "cm_adc.h"

int checkSpinComplete() {
    
    uart1_puts("Press key when spin is complete: ");
    uart1Rx();

    return 1;

}
