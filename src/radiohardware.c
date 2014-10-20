/*
	Hardware specific code for the MRF24J40 radio on a 2.4GHz
	RF Pictail board on an Explorer16 board.
*/

#include "radiohardware.h"	

void	RadioHW_Init()
{
    RADIO_CS_TRIS = 0;
    RADIO_CS = 1;
    RADIO_RESETn_TRIS = 0;
    RADIO_RESETn = 1;
    RADIO_INT_TRIS = 1;
    RADIO_WAKE_TRIS = 0;
    RADIO_WAKE = 1;

    RPINR0bits.INT1R = RADIO_INT_PIN_NUM;

    #if defined(HARDWARE_SPI)

    RPOR1bits.RP2R = 0b00111;       // output SDO1 to RP2
    RPINR20bits.SDI1R = 3;          // input SDI1 to RP3
    RPOR2bits.RP4R = 0b01000;       // output SCK1 to RP4

    SPICON1 = SPICON1_VAL;
    SPISTAT = SPISTAT_VAL;

    #else

    RADIO_SDI_TRIS = 1;
    RADIO_SDO_TRIS = 0;
    RADIO_SCK_TRIS = 0;
    RADIO_SPI_SDO = 0;
    RADIO_SPI_SCK = 0;   

    #endif
}
