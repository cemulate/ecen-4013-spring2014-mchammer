#include <stdlib.h>
#include <xc.h>

#include "cm_adc.h"

void configureADC(int analogport) {
    AD1CON1bits.AD12B = 1;              // 12-bit one-channel higher acc. mode
    AD1CON2bits.VCFG = 0;               // AVdd and AVss as + and - ref
    AD1CON3bits.ADCS = 0;               // T_AD = T_CY (sounds about right)

    AD1PCFGL = AD1PCFGL & (~(1 << analogport));      // Disable digital function on pin 'analogport'; make it analog

    AD1CHS0bits.CH0SA = analogport;     // Sample on analog pin 'analogport'
    AD1CON2bits.CHPS = 0;               // Convert "channel 0"

    AD1CON1bits.FORM = 0;               // Read buffer in integer form

    AD1CON1bits.ASAM = 0;               // Don't auto sample, manually set the SAMP bit to sample
    AD1CON1bits.SSRC = 0b111;           // Auto convert (conversion is ended when the module decides)

    AD1CON1bits.ADON = 1;               // Enable module
}

unsigned int readADCRaw() {
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

double readADCPercent() {
    return 100 * (readADCRaw() / 4096.0);
}